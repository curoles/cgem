/**@file
 * @author Igor Lesik
 * Convert Journaling Record System file (JRS) to text.
 *
 */
#include <stdlib.h>
#include <stdio.h>
//#include <sstream>
#include <assert.h>

#include "jrs.h"

using namespace gem;

static
void print_record(File& f, const jrs::Record& record)
{
    fprintf (f, "===[%lu:%lu]===\n", record.hdr.tick, record.hdr.seqId);
    fprintf (f, "UID :%u\n", record.hdr.userId);
    fprintf (f, "SIZE:%lu\n", record.hdr.size);

    for (size_t i = 0; i < record.hdr.size; ++i)
    {
        fprintf (f, "%3lu:%02x\n", i, record.body[i]);
    }
}

bool gem::jrs::convert_to_text(const char* in, const char* out)
{
    if (in ==nullptr or out == nullptr) return false;

    jrs::Journal journal;

    if (!journal.open_for_read (in))
    {
        printf ("Can't open input file %s\n", in);
        return false;
    }

    File out_file;
    if (!out_file.open (out, "w"))
    {
        printf ("Can't open output file %s\n", out);
        return false;
    }

    jrs::Record record;
    File::pos_t pos = 0;

    void (*print)(File& f, const jrs::Record& record) = print_record;

    print = print_record;

    while ((pos = journal.read (record, pos)) != File::NPOS)
    {
        (*print)(out_file, record);
    }

    journal.close ();

    return true;
}

