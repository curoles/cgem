/**
 * Zip files into zip-archive file.
 *
 * Authors:   Igor Lesik 2015
 * Copyright: (C) Igor Lesik
 *
 * License:
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See_Also:
 * http://nomad.so/2015/09/working-with-files-in-the-d-programming-language/
 */
module app.d.toolbox.zip;

//import std.file;
import std.stdio;
//import std.outbuffer;
import std.string;
import std.zip;
import core.stdc.errno;
import std.exception;

import cgem.d.io.file;
 
void zip(string[] args)
{
    try
    {
        auto zip = new ZipArchive();

        foreach (arg; args)
        {
            auto file_name = arg;

            try {
            bool res = cgem.d.io.file.openFile(file_name, "r", /*do*/(file) {

                auto archive_item = new ArchiveMember();
                archive_item.name = file_name;

                std.stdio.write("Read "~file_name~" ");
                foreach (buffer; file.byChunk(4024))
                {
                    std.stdio.write(".");
                    archive_item.expandedData = archive_item.expandedData ~ buffer;
                }

                std.stdio.write("zipping ");
                zip.addMember(archive_item);
                std.stdio.writeln("OK");


                return true;
            });
            }
            catch (ErrnoException ex)
            {
                switch(ex.errno)
                {
                case EPERM:
                case EACCES:
                    // Permission denied
                    break;
     
                case ENOENT:
                    writeln("File "~file_name~" does not exist");
                    break;
     
                default:
                    // Handle other errors
                    break;
                }
            }
        }

        std.file.write("test.zip", zip.build());
    }
    catch (ZipException ex)
    {
        // Handle errors
    }
}
