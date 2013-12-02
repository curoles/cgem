/**@file
 * @brief     Journaling Record System
 * @author    Igor Lesik 2011
 * @copyright (c) 2011 Igor Lesik
 *
 * JRS helps to orginize sequential records inside a file, making it a journal.
 * JRS has only 2 types of records: Record and Marker.
 * Markers are placed in some distance from each other
 * and make up an ordered list. Markers serve to
 * do fast rewinding.
 *
 * Journal structure:
 * [M][R][R][R]...[R][M][R]...[R][M][R]...
 *
 */
#ifndef JRS_H_INCLUDED
#define JRS_H_INCLUDED

#include <stdint.h>

#include "cgem/File.h"

namespace gem { namespace jrs {

typedef uint64_t seq_id_t;
typedef uint64_t tick_t;
typedef uint32_t user_id_t;

typedef uint32_t magic_t;
const magic_t MAGIC_M = 0xbadc0ffe;
const magic_t MAGIC_R = 0x0abcdef0;

/** Record is the base element of Journal, it stores data.
 *  Record header has a time stamp and size of the body.
 */
struct Record
{
    /// Record position in file
    File::pos_t file_pos;

#pragma pack(push,1)
    struct Hdr
    {
        magic_t magic;
        seq_id_t seqId;
        tick_t tick;
        user_id_t userId;
        size_t size;
    } hdr;
#pragma pack(pop)

    uint8_t* body;
    size_t body_alloc_size;

    Record();
    //Record(const Record& x);
    //Record& operator= (const Record& x);

    ~Record();

    /// Size of Record
    inline size_t size() const {
        return hdr.size + sizeof (hdr);
    }

    /// Record position plus size of the record give pointer to next record
    inline File::pos_t next_record() const {
        return file_pos + (File::pos_t)size ();
    }

    /// Read record from file
    bool read(File& file, File::pos_t pos);

    /// Write record to file
    bool write(File& file);

private:
    bool read_hdr(File& file, File::pos_t pos);
    bool read_body(File& file);
};

struct Marker
{
#pragma pack(push,1)
    struct JM
    {
        magic_t magic;
        seq_id_t seqId;
        tick_t tick;
        File::pos_t span;       ///< distance from the mark to next mark
        File::pos_t prev_marker;
    } mark;
#pragma pack(pop)

    File::pos_t file_pos;

    bool read(File& file, File::pos_t pos = 0);
    bool write(File& file);

    inline File::pos_t next_marker() const {
        return file_pos + mark.span;
    }

    inline File::pos_t prev_marker() const {
        return mark.prev_marker;
    }

    inline File::pos_t first_record() const {
        return file_pos + sizeof (mark);
    }
    //File::pos_t nextRecord(File& file, File::pos_t cur_pos) const;
};

class Journal
{
public:
    Journal();
    ~Journal();

    bool open_for_read(const char* filename, bool unbuffered = false);
    bool open_new(const char* filename, bool unbuffered = false);
    bool open_append(const char* filename);

    void close() {
        m_file.close ();
    }

    bool is_open() {
        return m_file.is_open ();
    }

    bool is_eof() {
        return m_file.is_eof ();
    }

    long int get_current_file_position() {
        return m_file.current_position ();
    }

    void rewind_to_beginning() {
        fflush (m_file);
        rewind (m_file);
    }

    bool add_record(
        size_t body_size,
        const uint8_t* body,
        tick_t tick = 0,
        user_id_t userId = 0);

    template <class T>
    bool add_record_val(
        T& val,
        tick_t tick = 0,
        user_id_t userId = 0)
    {
        return add_record (sizeof (val), (uint8_t*)&val, tick, userId);
    }

    File::pos_t read(Record& rec, File::pos_t pos);

    seq_id_t get_sequence_count() const {
        return m_seq_count;
    }

private:
    bool add_marker (tick_t);
    bool add_record(Record& record);

private:
    File m_file;
    seq_id_t m_seq_count;
    File::pos_t m_last_record_pos;

    static const seq_id_t MARKER_SPAN = 100;
};

bool convert_to_text(const char* fin, const char* fout);

}} // jrs


 #endif
