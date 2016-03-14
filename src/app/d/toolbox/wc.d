module app.d.toolbox.wc;

import std.stdio;
import std.algorithm;

int wc(string[] args)
{
    return classic_wordcount(args);
}

//http://dlang.org/wc.html
int classic_wordcount(string[] args)
{
    ulong wordCount;
    ulong lineCount;
    ulong charCount;

    int[string] dictionary;
    writeln("   lines   words   bytes file");

    foreach(arg; args[1 .. args.length])
    {
        ulong lWordCount;
        ulong lCharCount;
        ulong lLineCount;

        auto file = File(arg);
        foreach(line; file.byLine(KeepTerminator.yes))
        {
            lCharCount += line.length;
            foreach(char[] word; splitter(line))
            {
                lWordCount += 1;
                if(auto count = word in dictionary)
                    *count += 1;
                else
                    dictionary[word.idup] = 1;
            }

            lLineCount += 1;
        }

        writefln("%8s%8s%8s %s\n", lLineCount, lWordCount, lCharCount, arg);

        wordCount += lWordCount;
        lineCount += lLineCount;
        charCount += lCharCount;
    }

    if (args.length > 2)
    {
        writefln("--------------------------------------\n%8s%8s%8s total",
                        lineCount, wordCount, charCount);
    }

    writeln("--------------------------------------");

    foreach (word; sort(dictionary.keys))
    {
            writefln("%3s %s", dictionary[word], word);
    }
    return 0;
}

/*
void wc_with_ranges()
{
    auto file = File("file.txt"); // Open for reading
    const wordCount = file.byLine()            // Read lines
                          .map!split           // Split into words
                          .map!(a => a.length) // Count words per line
                          .sum();              // Total word count
    writeln(wordCount);
}
*/
