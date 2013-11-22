/**@file
 * @brief     Journaling record system
 * @author    Igor Lesik 2011
 * @copyright (c) 2011 Igor Lesik
 *
 * Sequential records organized in journal.
 */
#include "jrs.h"

#include <assert.h>

namespace jrs
{

Record::Record() :
    file_pos (File::NPOS),
    body (NULL),
    body_alloc_size (0)
{
    hdr.size = 0;
}


Record::Record(const Record& x) :
    file_pos (x.file_pos),
    hdr (x.hdr),
    body (NULL),
    body_alloc_size (x.body_alloc_size)
{
    if (body_alloc_size)
    {
        body = new uint8_t[body_alloc_size];
        memcpy (body, x.body, hdr.size);
    }
}

Record& Record::operator= (const Record& x)
{
    file_pos = x.file_pos;
    hdr = x.hdr;
    body_alloc_size = x.body_alloc_size;
    delete[] body;
    body = NULL;

    if (body_alloc_size)
    {
        body = new uint8_t[body_alloc_size];
        memcpy (body, x.body, hdr.size);
    }

    return *this;
}

Record::
~Record()
{
    if (body_alloc_size)
    {
        delete[] body;
    }
}

bool
Record::readHdr(File& file,
    File::pos_t pos)
{
    file_pos = pos;

    if (!file.isOpen ())
        return false;

    return fread_val (file, pos, hdr);
}

bool
Record::readBody(File& file)
{
    if (!body || hdr.size > body_alloc_size)
    {
        delete[] body;
        body = new uint8_t[hdr.size];
        if (!body)
            return false;
        body_alloc_size = hdr.size;
    }

    size_t received = fread ((void*)body, sizeof (body[0]), hdr.size, file);
    return received == hdr.size;
}

bool
Record::read(File& file,
    File::pos_t pos)
{
    //debugJrsRecord(cosim::Dbg::JOURNAL+11, "read pos %"PRId64", fd=%lx", pos, &file );

    if (!readHdr (file, pos))
    {
        return false;
    }

    if (hdr.magic != MAGIC_R)
    {
        assert (hdr.magic == MAGIC_M);
        if (!readHdr (file, pos + sizeof (Marker::mark)))
        {
            return false;
        }
        assert (hdr.magic == MAGIC_R);
    }

    return readBody (file);
}

bool
Record::write(File& file)
{
    file_pos = ftell (file);

    //debugJrsRecord(cosim::Dbg::JOURNAL+11, "write pos %"PRId64", fd=%lx", file_pos, &file );

    if (!fwrite_val (file, hdr))
    {
        return false;
    }

    assert (body != NULL);

    size_t written = fwrite ((void*)body, sizeof (body[0]), hdr.size, file);
    fflush (file);

    return written == hdr.size;
}

bool
Marker::write(File& file)
{
    file_pos = ftell (file);

    //debugJrsMarker(cosim::Dbg::JOURNAL+11, "write pos %"PRId64", fd=%lx", file_pos, &file );

    return fwrite_val (file, mark);
}

bool
Marker::read(File& file,
    File::pos_t pos)
{
    //debugJrsMarker(cosim::Dbg::JOURNAL+11, "read pos %"PRId64", fd=%lx", pos, &file );

    if (!fread_val (file, pos, mark))
        return false;

    if (mark.magic != jrs::MAGIC_M)
    {
        return false;
    }

    file_pos = pos;

    return true;
}

/*File::pos_t
   Marker::
   nextRecord(File& file, File::pos_t cur_pos) const
   {
    jrs::Record rec;
    return rec.readHdr(file, cur_pos)? rec.nextRecord() : File::NPOS;
   }*/

Journal::Journal(int id) :
    m_id (id),
    m_seq_count (0),
    m_last_record_pos (0)
{
    //jsr_debug_init();
}

Journal::~Journal()
{
    if (!m_file.isOpen ())
    {
        //debugJrsJournal(cosim::Dbg::JOURNAL, "done" );
        return;
    }

    //debugJrsJournal(cosim::Dbg::JOURNAL, "done, fd=%lx", &m_file );

    m_file.close ();
}

bool
Journal::openForRead(const char* filename,
    bool unbuffered)
{
    if (isOpen ())
        return false;

    if (!m_file.open (filename, "r"))
        return false;

    //debugJrsJournal(cosim::Dbg::JOURNAL, "openForRead \"%s\", fd=%lx", filename, &m_file );

    // Force unbuffered operation.
    if (unbuffered)
        setbuf (m_file, NULL);

    m_seq_count = 0;

    //seqCount = ; read last record and get seqId

    return true;
}

bool
Journal::openNew(const char* filename,
    bool unbuffered)
{
    if (isOpen ())
        return false;

    if (!m_file.open (filename, "w+"))
        return false;

    //debugJrsJournal(cosim::Dbg::JOURNAL, "openNew \"%s\", fd=%lx", filename, &m_file );

    // Force unbuffered operation.
    if (unbuffered)
        setbuf (m_file, NULL);

    m_seq_count = 0;

    return true;
}

bool
Journal::openAppend(const char* filename)
{
    if (isOpen ())
        return false;

    if (!m_file.open (filename, "a+"))
        return false;

    //debugJrsJournal(cosim::Dbg::JOURNAL, "openAppend \"%s\", fd=%lx", filename, &m_file );

    //seqCount = ;read last record and get seqId

    return true;
}


File::pos_t
Journal::read(Record& rec,
    File::pos_t pos)
{
    //debugJrsJournal(cosim::Dbg::JOURNAL, "read pos %"PRId64", fd=%lx", pos, &m_file );

    assert (isOpen ());

    if (pos == File::NPOS)
        return File::NPOS;

    if (!rec.read (m_file, pos))
    {
        //debugJrsJournal(cosim::Dbg::JOURNAL+12, "eof" );
        return File::NPOS;
    }

    return rec.nextRecord ();
}

bool
Journal::addRecord(
    size_t body_size,
    const uint8_t* body,
    tick_t tick,
    user_id_t userId)
{
    assert (isOpen ());

    //debugJrsJournal(cosim::Dbg::JOURNAL, "addRecord pos %"PRId64", fd=%lx", ftell(m_file), &m_file );

    if ((m_seq_count % MARKER_SPAN) == 0)
    {
        addMarker (tick);
    }

    Record record;
    record.hdr.magic  = MAGIC_R;
    record.hdr.seqId  = m_seq_count++;
    record.hdr.tick   = tick;
    record.hdr.userId = userId;
    record.hdr.size   = body_size;

    record.body = (uint8_t*)body;

    return addRecord (record);
}

bool
Journal::addRecord(Record& record)
{
    assert (isOpen ());

    return record.write (m_file);
}

bool
Journal::addMarker(tick_t tick)
{
    Marker marker;
    marker.mark.magic = MAGIC_M;
    marker.mark.seqId = m_seq_count;;
    marker.mark.tick  = tick;
    marker.mark.span  = File::NPOS;
    marker.mark.prev_marker = File::NPOS;

    return marker.write (m_file);
}

} // namespace jrs
