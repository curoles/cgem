#include "Backtrace.h"

#include <execinfo.h>
//#include <string.h>
#include <cstdlib>

void
gem::Backtrace::
get(std::function<void (int, const char*)> printer)
{
    void* buffer[256];

    int backtrace_size = backtrace(buffer, sizeof(buffer)/sizeof(buffer[0]));

    char** messages = backtrace_symbols(buffer, backtrace_size);

    if (messages == nullptr)
    {
        //perror ("backtrace_symbols");
    }
    else
    {
        for (int i = 0; i < backtrace_size; ++i)
        {
            printer(i, messages[i]);
        }
    }

    free(messages);
}

void
gem::Backtrace::
print(FILE* stream)
{
    gem::Backtrace::get([stream](int i, const char* str){
        fprintf(stream, "bt[%d] %s\n", i, str);
    });

}

void
gem::Backtrace::
print(std::ostream& stream)
{
    gem::Backtrace::get([&stream](int i, const char* str){
        stream << "bt[" << i << "]" << str << "\n";
    });

}

#if 0
void run_gdb_instance_to_show_backtrace()
{
    char pid_buf[32];
    snprintf (pid_buf, sizeof (pid_buf), "%d", getpid ());

    char process_name[512];
    process_name[readlink ("/proc/self/exe", process_name, sizeof (process_name) - 1)] = '\0';

    int child_pid = fork ();

    if (child_pid == -1)
    {
        debugCosim (cosim::Dbg::ALWAYS, "Could not start GDB");
        return;
    }

    bool is_child_process = (child_pid == 0);

    if (is_child_process)
    {
        dup2 (2, 1); // redirect output to stderr

        fprintf (stderr, "\nStack Trace for %s pid=%s", process_name, pid_buf);

        execlp ("gdb", "gdb", "--batch", "-n", "-ex", "thread", "-ex", "bt", process_name, pid_buf,
            NULL);

        //abort(); /* If gdb failed to start */
    }
    else // parent process
    {
        waitpid (child_pid, NULL, 0);
    }
}

#endif

