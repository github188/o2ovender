#include <iostream>
#include <string>

#include <unistd.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#include "SDHandlerManager.h"

#ifdef LOGGER
#include <log4cplus/configurator.h>
using namespace log4cplus;
#endif

using namespace std;

void sig_handler_usr1(int sig)
{
    //g_reload_hash_table_map = true;
}

void sig_handler_usr2(int sig)
{
    exit(0);
}

void sig_handler_hub(int sig)
{
    //SDDynamicConfig::get_instance()->reload();
}


void usage(const char* program)
{
    cout << program << " -f <config file> [-d]" << endl;
}

int main(int argc, char* argv[])
{
#ifdef LOGGER
    PropertyConfigurator::doConfigure("../conf/log4cplus.properties");
#endif

    string conf_file;

    extern int optopt;
    extern char* optarg;    
    int c;
    while ((c = getopt(argc, argv, "dvf:")) != -1) {
        switch (c) {
            case 'd':
                daemon(1, 1);
                break;

            case 'v':
                cout << "version: 1.0.0.1" << endl;
                break;

            case 'f':
                conf_file = optarg;
                break;

            case '?':
                cerr << "unrecognized option: -" << optopt << endl;
                usage(argv[0]);
                exit(1);
                break;

            default:
                cerr << "unrecognized option: -" << optopt << endl;
                usage(argv[0]);
                exit(1);
                break;
        }
    }

    if (conf_file.empty()) {
        cerr << "no config file" << endl;
        usage(argv[0]);
        exit(1);
    }

    signal(SIGPIPE, SIG_IGN);
    signal(SIGHUP, sig_handler_hub);
    signal(SIGUSR1, sig_handler_usr1);
    signal(SIGUSR2, sig_handler_usr2);

    SDHandlerManager server(conf_file);
    if (!server.init()) {
        exit(1);
    }
    server.start();
    server.wait();

    return 0;
}
