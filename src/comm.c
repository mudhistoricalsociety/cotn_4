/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Dystopia Mud improvements copyright (C) 2000, 2001 by Brian Graversen  *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/
/***************************************************************************
 *                                 _/                            _/        *
 *      _/_/_/  _/_/      _/_/_/  _/    _/_/    _/    _/    _/_/_/         *
 *     _/    _/    _/  _/        _/  _/    _/  _/    _/  _/    _/          *
 *    _/    _/    _/  _/        _/  _/    _/  _/    _/  _/    _/           *
 *   _/    _/    _/    _/_/_/  _/    _/_/      _/_/_/    _/_/_/            *
 ***************************************************************************
 * Mindcloud Copyright 2001-2003 by Jeff Boschee (Zarius),                 *
 * Additional credits are in the help file CODECREDITS                     *
 * All Rights Reserved.                                                    *
 ***************************************************************************/
/*
 * This file contains all of the OS-dependent stuff:
 *   startup, signals, BSD sockets for tcp/ip, i/o, timing.
 *
 * The data flow for input is:
 *    Game_loop ---> Read_from_descriptor ---> Read
 *    Game_loop ---> Read_from_buffer
 *
 * The data flow for output is:
 *    Game_loop ---> Process_Output ---> Write_to_descriptor -> Write
 *
 * The OS-dependent functions are Read_from_descriptor and Write_to_descriptor.
 * -- Furey  26 Jan 1993
 */

#include <sys/types.h>
#include <sys/time.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>   // we need these to call wait()
#include <execinfo.h>   // internal backtraces.

#include "merc.h"
#include "interp.h"
#include "olc.h"
#include "recycle.h"

/*
 * Malloc debugging stuff.
 */
#if defined(sun)
#undef MALLOC_DEBUG
#endif

#if defined(MALLOC_DEBUG)
#include <malloc.h>
extern int malloc_debug args((int));
extern int malloc_verify args((void));
#endif



/*
 * Signal handling.
 * Apollo has a problem with __attribute(atomic) in signal.h,
 *   I dance around it.
 */
#if defined(apollo)
#define __attribute(x)
#endif

#if defined(unix) || defined(WIN32)
#include <signal.h>
#if !defined(WIN32)
#include <unistd.h>
#include <sys/resource.h>   /* for RLIMIT_NOFILE */
#endif
#endif

#if defined(apollo)
#undef __attribute
#endif

/*
 * Socket and TCP/IP stuff.
 */
#if	defined(macintosh) || defined(MSDOS)
const char echo_off_str[] = { '\0' };
const char echo_on_str[] = { '\0' };
const char go_ahead_str[] = { '\0' };
const char compress_will[] = { '\0' };
const char compress_do[] = { '\0' };
const char compress_dont[] = { '\0' };
const char compress_start[] = { '\0' };
#endif

#if	defined(unix)
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/telnet.h>
const char echo_off_str[] = { IAC, WILL, TELOPT_ECHO, '\0' };
const char echo_on_str[] = { IAC, WONT, TELOPT_ECHO, '\0' };
const char go_ahead_str[] = { IAC, GA, '\0' };
const char compress_will[] = { IAC, WILL, TELOPT_COMPRESS, '\0' };
const char compress_do[] = { IAC, DO, TELOPT_COMPRESS, '\0' };
const char compress_dont[] = { IAC, DONT, TELOPT_COMPRESS, '\0' };

void show_string args((DESCRIPTOR_DATA * d, char *input));
int num_changes args((void));

#endif

#if	defined( WIN32 )
const char echo_off_str[] = { '\0' };
const char echo_on_str[] = { '\0' };
const char go_ahead_str[] = { '\0' };
void gettimeofday args((struct timeval * tp, void *tzp));
void show_string args((DESCRIPTOR_DATA * d, char *input));
#endif

/*
 * OS-dependent declarations.
 */
#if	defined(_AIX)
#include <sys/select.h>
int accept args((int s, struct sockaddr * addr, int *addrlen));
int bind  args((int s, struct sockaddr * name, int namelen));
void bzero args((char *b, int length));
int getpeername args((int s, struct sockaddr * name, int *namelen));
int getsockname args((int s, struct sockaddr * name, int *namelen));
int gettimeofday args((struct timeval * tp, struct timezone * tzp));
int listen args((int s, int backlog));
int setsockopt args((int s, int level, int optname, void *optval,
                     int optlen));
int socket args((int domain, int type, int protocol));
#endif

#if	defined(apollo)
#include <unistd.h>
void bzero args((char *b, int length));
#endif

#if	defined(__hpux)
int accept args((int s, void *addr, int *addrlen));
int bind  args((int s, const void *addr, int addrlen));
void bzero args((char *b, int length));
int getpeername args((int s, void *addr, int *addrlen));
int getsockname args((int s, void *name, int *addrlen));
int gettimeofday args((struct timeval * tp, struct timezone * tzp));
int listen args((int s, int backlog));
int setsockopt args((int s, int level, int optname,
                     const void *optval, int optlen));
int socket args((int domain, int type, int protocol));
#endif

#if	defined(interactive)
#include <net/errno.h>
#include <sys/fcntl.h>
#endif

#if	defined(linux)
//int   close       args( ( int fd ) );
//int   gettimeofday    args( ( struct timeval *tp, struct timezone *tzp ) );
//int   select      args( ( int width, fd_set *readfds, fd_set *writefds,
//              fd_set *exceptfds, struct timeval *timeout ) );
//int   socket      args( ( int domain, int type, int protocol ) );
#endif

#if	defined(macintosh)
#include <console.h>
#include <fcntl.h>
#include <unix.h>
struct timeval
{
        time_t    tv_sec;
        time_t    tv_usec;
};

#if	!defined(isascii)
#define	isascii(c)		( (c) < 0200 )
#endif
static long theKeys[4];

int gettimeofday args((struct timeval * tp, void *tzp));
#endif

#if	defined(MIPS_OS)
extern int errno;
#endif

#if	defined(MSDOS)
int gettimeofday args((struct timeval * tp, void *tzp));
int kbhit args((void));
#endif

#if	defined(NeXT)
int close args((int fd));
int fcntl args((int fd, int cmd, int arg));

#if	!defined(htons)
u_short htons args((u_short hostshort));
#endif
#if	!defined(ntohl)
u_long ntohl args((u_long hostlong));
#endif
int read  args((int fd, char *buf, int nbyte));
int select args((int width, fd_set * readfds, fd_set * writefds,
                 fd_set * exceptfds, struct timeval * timeout));
int write args((int fd, char *buf, int nbyte));
#endif

#if	defined(sequent)
int accept args((int s, struct sockaddr * addr, int *addrlen));
int bind  args((int s, struct sockaddr * name, int namelen));
int close args((int fd));
int fcntl args((int fd, int cmd, int arg));
int getpeername args((int s, struct sockaddr * name, int *namelen));
int getsockname args((int s, struct sockaddr * name, int *namelen));
int gettimeofday args((struct timeval * tp, struct timezone * tzp));

#if	!defined(htons)
u_short htons args((u_short hostshort));
#endif
int listen args((int s, int backlog));

#if	!defined(ntohl)
u_long ntohl args((u_long hostlong));
#endif
int read  args((int fd, char *buf, int nbyte));
int select args((int width, fd_set * readfds, fd_set * writefds,
                 fd_set * exceptfds, struct timeval * timeout));
int setsockopt args((int s, int level, int optname, caddr_t optval,
                     int optlen));
int socket args((int domain, int type, int protocol));
int write args((int fd, char *buf, int nbyte));
#endif

/*
 * This includes Solaris SYSV as well.
 */
#if defined(sun)
int accept args((int s, struct sockaddr * addr, int *addrlen));
int bind  args((int s, struct sockaddr * name, int namelen));
void bzero args((char *b, int length));
int close args((int fd));
int getpeername args((int s, struct sockaddr * name, int *namelen));
int getsockname args((int s, struct sockaddr * name, int *namelen));
int gettimeofday args((struct timeval * tp, struct timezone * tzp));
int listen args((int s, int backlog));
int read  args((int fd, char *buf, int nbyte));
int select args((int width, fd_set * readfds, fd_set * writefds,
                 fd_set * exceptfds, struct timeval * timeout));
#if defined(SYSV)
int setsockopt args((int s, int level, int optname,
                     const char *optval, int optlen));
#else
int setsockopt args((int s, int level, int optname, void *optval,
                     int optlen));
#endif
int socket args((int domain, int type, int protocol));
int write args((int fd, char *buf, int nbyte));
#endif

#if defined(ultrix)
int accept args((int s, struct sockaddr * addr, int *addrlen));
int bind  args((int s, struct sockaddr * name, int namelen));
void bzero args((char *b, int length));
int close args((int fd));
int getpeername args((int s, struct sockaddr * name, int *namelen));
int getsockname args((int s, struct sockaddr * name, int *namelen));
int gettimeofday args((struct timeval * tp, struct timezone * tzp));
int listen args((int s, int backlog));
int read  args((int fd, char *buf, int nbyte));
int select args((int width, fd_set * readfds, fd_set * writefds,
                 fd_set * exceptfds, struct timeval * timeout));
int setsockopt args((int s, int level, int optname, void *optval,
                     int optlen));
int socket args((int domain, int type, int protocol));
int write args((int fd, char *buf, int nbyte));
#endif

/*
 * Global variables.
 */
DESCRIPTOR_DATA *descriptor_free;   /* Free list for descriptors    */
DESCRIPTOR_DATA *descriptor_list;   /* All open descriptors     */
DESCRIPTOR_DATA *d_next;    /* Next descriptor in loop  */
FILE     *fpReserve;    /* Reserved file handle     */
bool      god;  /* All new chars are gods!  */
bool      merc_down;    /* Shutdown         */
bool      wizlock;  /* Game is wizlocked        */
char      str_boot_time[MAX_INPUT_LENGTH];
char      crypt_pwd[MAX_INPUT_LENGTH];
time_t    current_time; /* Time of this pulse       */
int       arena;
bool      quest;    /* Is there a quest running?    */
bool      auto_quest;   /* Quests start on their own?   */
CHAR_DATA *quest_mob;   /* Mob which started quest      */
CHAR_DATA *quest_target;    /* Target of the quest      */
OBJ_DATA *aquest_object;    /* Object to recover        */
int       quest_timer;  /* Time left to get object  */
int       quest_wait = 0;   /* Min time until next quest    */
sh_int    quest_personality;    /* mob's crusade personality :) */
bool      MOBtrigger = TRUE;    /* act() switch */

/* Colour scale char list - Calamar */

char     *scale[SCALE_COLS] = {
        L_RED,
        YELLOW,
        L_GREEN,
        L_BLUE
};

/*
 * OS-dependent local functions.
 */
#if defined(macintosh) || defined(MSDOS)
void game_loop_mac_msdos args((void));
bool read_from_descriptor args((DESCRIPTOR_DATA * d));
bool write_to_descriptor args((DESCRIPTOR_DATA * d, char *txt, int length));
bool write_to_descriptor_2 args((int desc, char *txt, int length));
#endif

#if defined(unix) || defined( WIN32 )
void game_loop_unix args((int control));
int init_socket args((int port));
void new_descriptor args((int control));
bool read_from_descriptor args((DESCRIPTOR_DATA * d));
#endif




/*
 * Other local functions (OS-independent).
 */
bool check_reconnect args((DESCRIPTOR_DATA * d, char *name, bool fConn));
bool check_kickoff args((DESCRIPTOR_DATA * d, char *name, bool fConn));
bool check_playing args((DESCRIPTOR_DATA * d, char *name));
int main  args((int argc, char **argv));
void nanny args((DESCRIPTOR_DATA * d, char *argument));
bool process_output args((DESCRIPTOR_DATA * d, bool fPrompt));
void read_from_buffer args((DESCRIPTOR_DATA * d));
void stop_idling args((CHAR_DATA * ch));
void bust_a_prompt args((DESCRIPTOR_DATA * d));
void bust_a_header args((DESCRIPTOR_DATA * d));

void lookup_address args((DUMMY_ARG * dummyarg));   // Only threaded calls, please.
bool check_banned args((DESCRIPTOR_DATA * dnew));   // Ban check
bool check_newbiebanned args((DESCRIPTOR_DATA * dnew)); // Ban check

int       proc_pid;
int       port, control;

int main(int argc, char **argv)
{
        struct timeval now_time;
        bool      fCopyOver = FALSE;
        char      buf[MSL];

        /*
         * Memory debugging if needed.
         */
#if defined(MALLOC_DEBUG)
        malloc_debug(2);
#endif

#ifdef RLIMIT_NOFILE
#ifndef min
# define min(a,b)     (((a) < (b)) ? (a) : (b))
#endif
        {
                struct rlimit rlp;

                (void) getrlimit(RLIMIT_NOFILE, &rlp);
                rlp.rlim_cur = min(rlp.rlim_max, FD_SETSIZE);
                (void) setrlimit(RLIMIT_NOFILE, &rlp);
        }
#endif

        /*
         * Crash recovery by Mandrax
         */

        signal(SIGSEGV, crashrecov);
/*    signal(SIGSEGV, iBacktrace); */
        signal(SIGTERM, panic_shutdown);

        proc_pid = getpid();

        /*
         * Init time and encryption.
         */
        gettimeofday(&now_time, NULL);
        current_time = (time_t) now_time.tv_sec;
        strcpy(str_boot_time, ctime(&current_time));
        xprintf(crypt_pwd, "Don't bother.");

        /*
         * Macintosh console initialization.
         */
#if defined(macintosh)
        console_options.nrows = 31;
        cshow(stdout);
        csetmode(C_RAW, stdin);
        cecho2file("log file", 1, stderr);
#endif

        /*
         * Reserve one channel for our use.
         */
        if ((fpReserve = fopen(NULL_FILE, "r")) == NULL)
        {
                perror(NULL_FILE);
                exit(1);
        }

        /*
         * Get the port number.
         */
        port = 4000;
        if (argc > 1)
        {
                if (!is_number(argv[1]))
                {
                        fprintf(stderr, "Usage: %s [port #]\n", argv[0]);
                        exit(1);
                }
                else if ((port = atoi(argv[1])) <= 1024)
                {
                        fprintf(stderr, "Port number must be above 1024.\n");
                        exit(1);
                }
        }

        if (argv[2] && argv[2][0])
        {
                fCopyOver = TRUE;
                control = atoi(argv[3]);
#ifdef I3
                I3_socket = atoi(argv[4]);
#endif
        }

        else
                fCopyOver = FALSE;
        /*
         * Run the game.
         */
#if defined(macintosh) || defined(MSDOS)
        boot_db();
        log_string(LOG_GAME, "Mindcloud is ready to rock.");
        game_loop_mac_msdos();
#endif

#if defined(unix) || defined(WIN32)
#ifdef I3
        I3_main(FALSE, port, fCopyOver);
#endif
#ifdef IMC
        imc_startup(FALSE); // FALSE arg, so the autoconnect setting can govern it.
#endif

        if (!fCopyOver) /* We have already the port if copyover'ed */
                control = init_socket(port);
        boot_db(fCopyOver);

        xprintf(buf, "Mindcloud is ready to rock on port %d.", port);
        wiznet(buf, NULL, NULL, WIZ_DEBUG, 0, 0);
        log_string(LOG_GAME, buf);
        game_loop_unix(control);
#if !defined( WIN32 )
        close(control);
#ifdef I3
        I3_shutdown(0);
#endif
#ifdef IMC
        imc_shutdown(FALSE);
#endif
#else
        closesocket(control);
        WSACleanup();
#endif
#endif
//        endchat();

        /*
         * That's all, folks.
         */
        log_string(LOG_GAME, "Normal termination of game.");
        xprintf_2(buf, "Normal termination of game.");
        wiznet(buf, NULL, NULL, WIZ_DEBUG, 0, 0);
        exit(0);
        return 0;
}

int init_socket(int sport)
{
        static struct sockaddr_in sa_zero;
        struct sockaddr_in sa;
        int       x = 1;
        int       fd;

        if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
                perror("Init_socket: socket");
                exit(1);
        }

        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &x, sizeof(int)) < 0)
        {
                perror("Init_socket: SO_REUSEADDR");
                close(fd);
                exit(1);
        }

        sa = sa_zero;
        sa.sin_family = AF_INET;
        sa.sin_port = htons(sport);

        if (bind(fd, (struct sockaddr *) &sa, sizeof(sa)) < 0)
        {
                perror("Init_socket: bind");
                close(fd);
                exit(1);
        }

        if (listen(fd, 3) < 0)
        {
                perror("Init_socket: listen");
                close(fd);
                exit(1);
        }

        return fd;
}

#if defined(macintosh) || defined(MSDOS)
void game_loop_mac_msdos(void)
{
        struct timeval last_time;
        struct timeval now_time;
        static DESCRIPTOR_DATA dcon;

        gettimeofday(&last_time, NULL);
        current_time = (time_t) last_time.tv_sec;

        /*
         * New_descriptor analogue.
         */
        dcon.descriptor = 0;
        dcon.connected = CON_GET_NAME;
        dcon.host = str_dup("localhost");
        dcon.outsize = 2000;
        dcon.outbuf = alloc_mem(dcon.outsize);
        dcon.next = descriptor_list;
        descriptor_list = &dcon;

        /*
         * Send the greeting.
         */
        /*
         * {
         * extern char * help_greeting;
         * if ( help_greeting[0] == '.' )
         * write_to_buffer( &dcon, help_greeting+1, 0 );
         * else
         * write_to_buffer( &dcon, help_greeting  , 0 );
         * }
         */

        /*
         * Send the greeting.
         */
        {
                extern char *help_greetinga;
                extern char *help_greetingb;

                switch (number_range(0, 2))
                {
                default:
                        if (help_greetinga[0] == '.')
                                write_to_buffer(dnew, help_greetinga + 1, 0);
                        else
                                write_to_buffer(dnew, help_greetinga, 0);
                        break;
                case 0:
                        if (help_greetinga[0] == '.')
                                write_to_buffer(dnew, help_greetinga + 1, 0);
                        else
                                write_to_buffer(dnew, help_greetinga, 0);
                        break;
                case 1:
                        if (help_greetingb[0] == '.')
                                write_to_buffer(dnew, help_greetingb + 1, 0);
                        else
                                write_to_buffer(dnew, help_greetingb, 0);
                        break;
                case 2:
                        if (help_greetingb[0] == '.')
                                write_to_buffer(dnew, help_greetingb + 1, 0);
                        else
                                write_to_buffer(dnew, help_greetingb, 0);
                        break;
                }
        }


        /*
         * Main loop 
         */
        while (!merc_down)
        {
                DESCRIPTOR_DATA *d;

                /*
                 * Process input.
                 */
                for (d = descriptor_list; d != NULL; d = d_next)
                {
                        d_next = d->next;
                        d->fcommand = FALSE;

#if defined(MSDOS)
                        if (kbhit())
#endif
                        {
                                if (d->character != NULL)
                                        d->character->timer = 0;
                                if (!read_from_descriptor(d))
                                {
                                        if (d->character != NULL)
                                                save_char_obj(d->character);
                                        d->outtop = 0;
                                        close_socket(d);
                                        continue;
                                }
                        }

                        if (d->character != NULL && d->character->wait > 0)
                        {
                                --d->character->wait;
                                continue;
                        }

                        read_from_buffer(d);
                        if (d->incomm[0] != '\0')
                        {
                                d->fcommand = TRUE;
                                stop_idling(d->character);

                                /*
                                 * OLC 
                                 */
                                if (d->showstr_point)
                                        show_string(d, d->incomm);
                                else if (d->pString)
                                        string_add(d->character, d->incomm);
                                else
                                        switch (d->connected)
                                        {
                                        default:
                                                nanny(d, d->incomm);
                                                break;
                                        case CON_PLAYING:
                                                if (!run_olc_editor(d))
                                                        interpret(d->
                                                                  character,
                                                                  d->incomm);
                                                break;
                                        case CON_EDITING:
                                                edit_buffer(d->character,
                                                            d->incomm);
                                                break;
                                        case CON_PFILE:
                                                jope_interp(d->character,
                                                            d->incomm);
                                                break;
                                        }

                                d->incomm[0] = '\0';
                        }
                }

#ifdef I3
                I3_loop();
#endif

                /*
                 * Autonomous game motion.
                 */
                update_handler();



                /*
                 * Output.
                 */
                for (d = descriptor_list; d != NULL; d = d_next)
                {
                        d_next = d->next;

                        if ((d->fcommand || d->outtop > 0 || d->out_compress)
                            && FD_ISSET(d->descriptor, &out_set)
                            if (!process_output(d, TRUE))
                            {
                            bool ok = TRUE;
                            if (d->fcommand || d->outtop > 0)
                            ok = process_output(d, TRUE);
                            if (ok && d->out_compress)
                            ok = processCompressed(d); if (!ok)
                            {
                            if (d->character != NULL)
                            save_char_obj(d->character);
                            d->outtop = 0; close_socket(d);}
                            }
                            }



                            /*
                             * Synchronize to a clock.
                             * Busy wait (blargh).
                             */
                            now_time = last_time; for (;;)
                            {
                            int delta;
#if defined(MSDOS)
                            if (kbhit())
#endif
                            {
                            if (dcon.character != NULL)
                            dcon.character->timer = 0;
                            if (!read_from_descriptor(&dcon))
                            {
                            if (dcon.character != NULL)
                            save_char_obj(d->character);
                            dcon.outtop = 0; close_socket(&dcon);}
#if defined(MSDOS)
                            break;
#endif
                            }

                            gettimeofday(&now_time, NULL);
                            delta =
                            (now_time.tv_sec -
                             last_time.tv_sec) * 1000 * 1000 +
                            (now_time.tv_usec - last_time.tv_usec);
                            if (delta >= 1000000 / PULSE_PER_SECOND) break;}
                            last_time = now_time;
                            current_time = (time_t) last_time.tv_sec;}

                            return;}
#endif



#if defined(unix) || defined(WIN32)

                            void excessive_cpu(int blx)
                            {
                            CHAR_DATA * vch;
                            CHAR_DATA * vch_next;
                            for (vch = char_list; vch != NULL; vch = vch_next)
                            {
                            vch_next = vch->next; if (!IS_NPC(vch))
                            {
                            send_to_char
                            ("Mud frozen: Autosave and quit.  The mud will reboot in 2 seconds.\n\r",
                             vch); interpret(vch, "quit");}
                            }
                            exit(1);}

                            void game_loop_unix(int gcontrol)
                            {
                            static struct timeval null_time;
                            struct timeval last_time;
#if !defined(WIN32)
                            signal(SIGPIPE, SIG_IGN);
#endif
                            gettimeofday(&last_time, NULL);
                            current_time = (time_t) last_time.tv_sec;
                            /*
                             * Main loop 
                             */
                            while (!merc_down)
                            {
                            fd_set in_set;
                            fd_set out_set;
                            fd_set exc_set; DESCRIPTOR_DATA * d; int maxdesc;
#if defined(MALLOC_DEBUG)
                            if (malloc_verify() != 1) abort();
#endif
                            /*
                             * Poll all active descriptors.
                             */
                            FD_ZERO(&in_set);
                            FD_ZERO(&out_set);
                            FD_ZERO(&exc_set);
                            FD_SET(control, &in_set);
                            maxdesc = gcontrol;
                            for (d = descriptor_list; d != NULL; d = d->next)
                            {
                            maxdesc = UMAX(maxdesc, d->descriptor);
                            FD_SET(d->descriptor, &in_set);
                            FD_SET(d->descriptor, &out_set);
                            FD_SET(d->descriptor, &exc_set);}

                            if (select
                                (maxdesc + 1, &in_set, &out_set, &exc_set,
                                 &null_time) < 0)
                            {
                            perror("Game_loop: select: poll"); exit(1);}

                            /*
                             * New connection?
                             */
                            if (FD_ISSET(gcontrol, &in_set))
                            new_descriptor(gcontrol);
                            /*
                             * Kick out the freaky folks.
                             */
                            for (d = descriptor_list; d != NULL; d = d_next)
                            {
                            d_next = d->next;
                            if (FD_ISSET(d->descriptor, &exc_set))
                            {
                            FD_CLR(d->descriptor, &in_set);
                            FD_CLR(d->descriptor, &out_set);
                            if (d->character)
                            save_char_obj(d->character);
                            d->outtop = 0; close_socket(d);}
                            }

                            /*
                             * Process input.
                             */
                            for (d = descriptor_list; d != NULL; d = d_next)
                            {
                            d_next = d->next;
                            d->fcommand = FALSE;
                            if (FD_ISSET(d->descriptor, &in_set))
                            {
                            if (d->character != NULL)
                            d->character->timer = 0;
                            if (!read_from_descriptor(d))
                            {
                            FD_CLR(d->descriptor, &out_set);
                            if (d->character != NULL)
                            save_char_obj(d->character);
                            d->outtop = 0; close_socket(d); continue;}
                            }

                            if (d->character != NULL
                                && d->character->wait > 0)
                            {
                            --d->character->wait; continue;}

                            read_from_buffer(d); if (d->incomm[0] != '\0')
                            {
                            d->fcommand = TRUE; stop_idling(d->character);
                            /*
                             * OLC 
                             */
                            if (d->showstr_point) show_string(d, d->incomm);
                            else
                            if (d->pString)
                            string_add(d->character, d->incomm);
                            else
                            switch (d->connected)
                            {
default:
nanny(d, d->incomm); break; case CON_PLAYING:
if (!run_olc_editor(d)) interpret(d->character, d->incomm); break; case CON_EDITING:
edit_buffer(d->character, d->incomm); break; case CON_PFILE:
                            jope_interp(d->character, d->incomm); break;}

                            d->incomm[0] = '\0';}
                            }

#ifdef I3
                            I3_loop();
#endif
#ifdef IMC
                            imc_loop();
#endif
                            /*
                             * Autonomous game motion.
                             */
                            update_handler();
                            //mud_recv_message();
                            /*
                             * Output.
                             */
                            for (d = descriptor_list; d != NULL; d = d_next)
                            {
                            d_next = d->next;
                            if ((d->fcommand || d->outtop > 0)
                                && FD_ISSET(d->descriptor, &out_set))
                            {
                            if (!process_output(d, TRUE))
                            {
                            if (d->character != NULL)
                            save_char_obj(d->character);
                            d->outtop = 0; close_socket(d);}
                            }
                            }



                            /*
                             * Synchronize to a clock.
                             * Sleep( last_time + 1/PULSE_PER_SECOND - now ).
                             * Careful here of signed versus unsigned arithmetic.
                             */
#if !defined(WIN32)
                            {
                            struct timeval now_time;
                            long secDelta;
                            long usecDelta;
                            gettimeofday(&now_time, NULL);
                            usecDelta =
                            ((int) last_time.tv_usec) -
                            ((int) now_time.tv_usec) +
                            1000000 / PULSE_PER_SECOND;
                            secDelta =
                            ((int) last_time.tv_sec) -
                            ((int) now_time.tv_sec); while (usecDelta < 0)
                            {
                            usecDelta += 1000000; secDelta -= 1;}

                            while (usecDelta >= 1000000)
                            {
                            usecDelta -= 1000000; secDelta += 1;}

                            if (secDelta > 0
                                || (secDelta == 0 && usecDelta > 0))
                            {
                            struct timeval stall_time;
                            stall_time.tv_usec = usecDelta;
                            stall_time.tv_sec = secDelta;
                            if (select(0, NULL, NULL, NULL, &stall_time) < 0)
                            {
                            perror("Game_loop: select: stall"); exit(1);}
                            }
                            }
#else
                            {
                            int times_up;
                            int nappy_time;
                            struct _timeb start_time;
                            struct _timeb end_time;
                            _ftime(&start_time);
                            times_up = 0; while (times_up == 0)
                            {
                            _ftime(&end_time);
                            if ((nappy_time =
                                 (int) (1000 *
                                        (double) ((end_time.time -
                                                   start_time.time) +
                                                  ((double)
                                                   (end_time.millitm -
                                                    start_time.millitm) /
                                                   1000.0)))) >=
                                (double) (1000 / PULSE_PER_SECOND)) times_up =
                            1;
                            else
                            {
                            Sleep((int) ((double) (1000 / PULSE_PER_SECOND) -
                                         (double) nappy_time)); times_up = 1;}
                            }
                            }
#endif

                            gettimeofday(&last_time, NULL);
                            current_time = (time_t) last_time.tv_sec;}

                            return;}
#endif



                            void init_descriptor(DESCRIPTOR_DATA * dnew,
                                                 int desc)
                            {
                            static DESCRIPTOR_DATA d_zero; *dnew = d_zero; dnew->descriptor = desc; dnew->character = NULL; dnew->connected = CON_GET_NAME; dnew->lookup_status = STATUS_LOOKUP; dnew->showstr_head = str_dup(""); dnew->showstr_point = 0; dnew->pEdit = NULL; /* OLC */
                            dnew->pString = NULL;   /* OLC */
                            dnew->editor = 0;   /* OLC */
                            dnew->outsize = 2000;
                            dnew->outbuf = alloc_mem(dnew->outsize);}

#if defined(unix) || defined(WIN32)
                            void new_descriptor(int dcontrol)
                            {
                            char buf[MAX_STRING_LENGTH];
                            DESCRIPTOR_DATA * dnew;
                            struct sockaddr_in sock;
                            int desc;
                            int size;
                            pthread_attr_t attr;
                            pthread_t thread_lookup;
                            DUMMY_ARG * dummyarg;
                            bool DOS_ATTACK = FALSE;
                            pthread_attr_init(&attr);
                            pthread_attr_setdetachstate(&attr,
                                                        PTHREAD_CREATE_DETACHED);
                            /*
                             * New Dummy Argument 
                             */
                            if (dummy_free == NULL)
                            {
                            dummyarg = alloc_perm(sizeof(*dummyarg));}
                            else
                            {
                            dummyarg = dummy_free;
                            dummy_free = dummy_free->next;}
                            dummyarg->status = 1;
                            dummyarg->next = dummy_list;
                            dummy_list = dummyarg;
                            size = sizeof(sock);
                            getsockname(dcontrol, (struct sockaddr *) &sock,
                                        &size);
                            if ((desc =
                                 accept(dcontrol, (struct sockaddr *) &sock,
                                        &size)) < 0)
                            {
                            perror("New_descriptor: accept"); return;}

#if !defined(FNDELAY)
#define FNDELAY O_NDELAY
#endif

#if !defined(WIN32)
                            if (fcntl(desc, F_SETFL, FNDELAY) == -1)
                            {
                            perror("New_descriptor: fcntl: FNDELAY"); return;}
#endif

                            /*
                             * Cons a new descriptor.
                             */
                            if (descriptor_free == NULL)
                            {
                            dnew = alloc_perm(sizeof(*dnew));}
                            else
                            {
                            dnew = descriptor_free;
                            descriptor_free = descriptor_free->next;}

                            init_descriptor(dnew, desc);
                            size = sizeof(sock);
                            if (getpeername
                                (desc, (struct sockaddr *) &sock, &size) < 0)
                            {
                            perror("New_descriptor: getpeername");
                            dnew->host = str_dup("(unknown)");}
                            else
                            {
                            /*
                             * Would be nice to use inet_ntoa here but it takes a struct arg,
                             * which ain't very compatible between gcc and system libraries.
                             */
                            int addr; addr = ntohl(sock.sin_addr.s_addr); xprintf(buf, "%d.%d.%d.%d", (addr >> 24) & 0xFF, (addr >> 16) & 0xFF, (addr >> 8) & 0xFF, (addr) & 0xFF); dnew->host = str_dup(buf);  // set the temporary ip as the host.
                            /*
                             * we do a hostlookup on everyone but localhost 
                             */
                            if (str_cmp(dnew->host, "127.0.0.1"))
                            {
                            /*
                             * Set up the dummyarg for use in lookup_address 
                             */
                            dummyarg->buf = str_dup((char *) &sock.sin_addr); dummyarg->d = dnew; if (thread_count < 50)    /* should be more than plenty */
                            {
                            /*
                             * just use the ip, then make the thread do the lookup 
                             */
                            pthread_create(&thread_lookup, &attr,
                                           (void *) &lookup_address,
                                           (void *) dummyarg);}
                            else
                            DOS_ATTACK = TRUE;}
                            else
                            dnew->lookup_status++;}

                            /*
                             * Init descriptor data.
                             */
                            dnew->next = descriptor_list;
                            descriptor_list = dnew; if (DOS_ATTACK)
                            {
                            write_to_buffer(dnew,
                                            "Sorry, currently under attack, try again later.\n\r",
                                            0); close_socket(dnew); return;}

                            /*
                             * mccp: tell the client we support compression 
                             */
                            write_to_buffer(dnew, compress_will, 0);
                            /*
                             * send greeting 
                             */
                            //{
                            //extern char * help_greeting;
                            //if ( help_greeting[0] == '.' )
                            //    write_to_buffer( dnew, help_greeting+1, 0 );
                            //else
                            //    write_to_buffer( dnew, help_greeting  , 0 );
                            //}
                            /*
                             * Send the greeting.
                             */
                            {
                            extern char *help_greetinga;
                            extern char *help_greetingb;
                            switch (number_range(0, 2))
                            {
default:
                            if (help_greetinga[0] == '.')
                            write_to_buffer(dnew, help_greetinga + 1, 0);
                            else
write_to_buffer(dnew, help_greetinga, 0); break; case 0:
                            if (help_greetinga[0] == '.')
                            write_to_buffer(dnew, help_greetinga + 1, 0);
                            else
write_to_buffer(dnew, help_greetinga, 0); break; case 1:
                            if (help_greetingb[0] == '.')
                            write_to_buffer(dnew, help_greetingb + 1, 0);
                            else
write_to_buffer(dnew, help_greetingb, 0); break; case 2:
                            if (help_greetingb[0] == '.')
                            write_to_buffer(dnew, help_greetingb + 1, 0);
                            else
                            write_to_buffer(dnew, help_greetingb, 0); break;}
                            }

                            if (DOS_ATTACK)
                            {
                            write_to_buffer(dnew,
                                            "Sorry, currently under attack, try again later.\n\r",
                                            0); close_socket(dnew);}
                            return;}
#endif

                            void lookup_address(DUMMY_ARG * darg)
                            {
                            struct hostent * from = 0; struct hostent ent; char buf[16384]; // enough ??
                            int err; thread_count++;
                            /*
                             * Removed for cygwin 
                             */
                            gethostbyaddr_r(darg->buf, sizeof(darg->buf),
                                            AF_INET, &ent, buf, 16384, &from,
                                            &err); if (from && from->h_name)
                            {
                            free_string(darg->d->host);
                            darg->d->host = str_dup(from->h_name);}

                            /*
                             * Brilliant system there Mr. Jobo
                             */
                            darg->d->lookup_status++;
                            free_string(darg->buf);
                            darg->status = 0;
                            thread_count--; pthread_exit(0);}


                            bool check_banned(DESCRIPTOR_DATA * dnew)
                            {
                            BAN_DATA * pban;
                            for (pban = ban_list; pban != NULL;
                                 pban =
                                 pban->next) if (!str_suffix(pban->name,
                                                             dnew->
                                                             host)) return
                            TRUE; return FALSE;}

                            bool check_newbiebanned(DESCRIPTOR_DATA * dnew)
                            {
                            BAN_DATA * pban;
                            for (pban = newbieban_list; pban != NULL;
                                 pban =
                                 pban->next) if (!str_suffix(pban->name,
                                                             dnew->
                                                             host)) return
                            TRUE; return FALSE;}

                            void close_socket(DESCRIPTOR_DATA * dclose)
                            {
                            CHAR_DATA * ch;
                            char buf[MSL];
                            if (dclose->lookup_status > STATUS_DONE) return;
                            dclose->lookup_status += 2;
                            if (dclose->outtop > 0) process_output(dclose,
                                                                   FALSE);
                            if (dclose->snoop_by !=
                                NULL) write_to_buffer(dclose->snoop_by,
                                                      "Your victim has left the game.\n\r",
                                                      0);
                            if (dclose->character != NULL
                                && (dclose->connected == CON_PLAYING
                                    || dclose->connected == CON_EDITING)
                                && IS_NPC(dclose->
                                          character)) do_return(dclose->
                                                                character,
                                                                "");
                            if (dclose->connected ==
                                CON_PFILE) jope_done(dclose->character, "");
                            /*
                             * Clear snoops
                             */
                            {
                            DESCRIPTOR_DATA * d;
                            for (d = descriptor_list; d != NULL; d = d->next)
                            if (d->snoop_by == dclose) d->snoop_by = NULL;}

                            /*
                             * Loose link or free char
                             */
                            if ((ch = dclose->character) != NULL)
                            {
                            xprintf(buf, "Closing link to %s.", ch->name);
                            log_string(LOG_CONNECT, buf);
                            wiznet(buf, ch, NULL, WIZ_LINKS, 0,
                                   get_trust(ch));
                            /*
                             * If ch is writing note or playing, just lose link otherwise clear char 
                             */
                            if ((dclose->connected == CON_PLAYING)
                                || ((dclose->connected >= CON_NOTE_TO)
                                    && (dclose->connected <=
                                        CON_NOTE_FINISH)))
                            {
                            if (IS_SET(ch->extra, EXTRA_AFK)) REMOVE_BIT(ch->
                                                                         extra,
                                                                         EXTRA_AFK);
                            if (IS_SET(ch->extra, EXTRA_OSWITCH))
                            do_humanform(ch, ""); do_call(ch, "all");
                            act("$n has lost $s link.", ch, NULL, NULL,
                                TO_ROOM);
                            wiznet("Net death has claimed $N.", ch, NULL,
                                   WIZ_LINKS, 0, get_trust(ch));
                            ch->desc = NULL;}
                            else
                            {
                            free_char(dclose->character);}
                            }
                            if (d_next == dclose) d_next = d_next->next;
                            dclose->connected = CON_NOT_PLAYING; return;}

/* For a better kickoff message :) KaVir */
                            void close_socket2(DESCRIPTOR_DATA * dclose,
                                               bool kickoff)
                            {
                            CHAR_DATA * ch;
                            if (dclose->lookup_status > STATUS_DONE) return;
                            dclose->lookup_status += 2;
                            if (dclose->outtop > 0) process_output(dclose,
                                                                   FALSE);
                            if (dclose->snoop_by !=
                                NULL) write_to_buffer(dclose->snoop_by,
                                                      "Your victim has left the game.\n\r",
                                                      0);
                            if (dclose->character != NULL
                                && (dclose->connected == CON_PLAYING
                                    || dclose->connected == CON_EDITING)
                                && IS_NPC(dclose->
                                          character)) do_return(dclose->
                                                                character,
                                                                "");
                            if (dclose->connected ==
                                CON_PFILE) jope_done(dclose->character, "");
                            /*
                             * Clear snoops
                             */
                            {
                            DESCRIPTOR_DATA * d;
                            for (d = descriptor_list; d != NULL; d = d->next)
                            if (d->snoop_by == dclose) d->snoop_by = NULL;}

                            if ((ch = dclose->character) != NULL)
                            {
                            if (dclose->connected == CON_PLAYING
                                || dclose->connected == CON_EDITING)
                            {
                            if (kickoff)
                            act
                            ("$n doubles over in agony and $s eyes roll up into $s head.",
                             ch, NULL, NULL, TO_ROOM); save_char_obj(ch);
                            ch->desc = NULL;}
                            else
                            {
                            free_char(dclose->character);}
                            }
                            if (d_next == dclose) d_next = d_next->next;
                            dclose->connected = CON_NOT_PLAYING; return;}


                            bool read_from_descriptor(DESCRIPTOR_DATA * d)
                            {
                            int iStart; char buf[MSL];
                            /*
                             * Hold horses if pending command already. 
                             */
                            if (d->incomm[0] != '\0') return TRUE;
                            /*
                             * one dirty patch to avoid spams of EOF's 
                             */
                            if (d->connected == CON_NOT_PLAYING) return TRUE;
                            /*
                             * Check for overflow. 
                             */
                            iStart = strlen(d->inbuf);
                            if (iStart >= sizeof(d->inbuf) - 10)
                            {
                            if (d != NULL && d->character != NULL)
                            {
                            xprintf(buf, "%s input overflow!",
                                    d->character->lasthost);
                            wiznet(buf, NULL, NULL, WIZ_DEBUG, 0, 0);
                            log_string(LOG_COMMAND, buf);}
                            else
                            if (d->lookup_status != STATUS_LOOKUP)
                            {
                            xprintf(buf, "%s input overflow!", d->host);
                            wiznet(buf, NULL, NULL, WIZ_DEBUG, 0, 0);
                            log_string(LOG_COMMAND, buf);}
                            write_to_descriptor(d,
                                                "\n\r*** PUT A LID ON IT!!! ***\n\r",
                                                0); return FALSE;}

                            /*
                             * Snarf input. 
                             */
#if defined(macintosh)
                            for (;;)
                            {
                            int c;
                            c = getc(stdin);
                            if (c == '\0' || c == EOF)
                            break;
                            putc(c, stdout);
                            if (c == '\r')
                            putc('\n', stdout);
                            d->inbuf[iStart++] = c;
                            if (iStart > sizeof(d->inbuf) - 10) break;}
#endif

#if defined(MSDOS) || defined(unix) || defined(WIN32)
                            for (;;)
                            {
                            int nRead;
#if !defined(WIN32)
                            nRead = read(d->descriptor, d->inbuf + iStart,
                                         sizeof(d->inbuf) - 10 - iStart);
#else
                            nRead = recv(d->descriptor, d->inbuf + iStart,
                                         sizeof(d->inbuf) - 10 - iStart, 0);
#endif
                            if (nRead > 0)
                            {
                            iStart += nRead;
                            if (d->inbuf[iStart - 1] == '\n'
                                || d->inbuf[iStart - 1] == '\r') break;}
                            else
                            if (nRead == 0)
                            {
                            return FALSE;}
#if !defined(WIN32)
                            else
                            if (errno == EWOULDBLOCK) break;
#endif
#if defined( WIN32 )
                            else
                            if (WSAGetLastError() == WSAEWOULDBLOCK
                                || errno == EAGAIN) break;
#endif
                            else
                            {
                            perror("Read_from_descriptor"); return FALSE;}
                            }
#endif

                            d->inbuf[iStart] = '\0'; return TRUE;}



/*
 * Transfer one line from input buffer to input line.
 */
                            void read_from_buffer(DESCRIPTOR_DATA * d)
                            {
                            int i, j, k; char buf[MSL];
                            /*
                             * Hold horses if pending command already.
                             */
                            if (d->incomm[0] != '\0') return;
                            /*
                             * Look for at least one new line.
                             */
                            for (i = 0;
                                 d->inbuf[i] != '\n' && d->inbuf[i] != '\r';
                                 i++)
                            {
                            if (d->inbuf[i] == '\0') return;}

                            /*
                             * Canonical input processing.
                             */
                            for (i = 0, k = 0;
                                 d->inbuf[i] != '\n' && d->inbuf[i] != '\r';
                                 i++)
                            {
                            if (k >= MAX_INPUT_LENGTH - 2)
                            {
                            write_to_descriptor(d, "Line too long.\n\r", 0);
                            /*
                             * skip the rest of the line 
                             */
                            for (; d->inbuf[i] != '\0'; i++)
                            {
                            if (d->inbuf[i] == '\n' || d->inbuf[i] == '\r')
                            break;}
                            d->inbuf[i] = '\n'; d->inbuf[i + 1] = '\0'; break;}
                            if (d->inbuf[i] == '\b' && k > 0)-- k;
                            else
                            if (isascii(d->inbuf[i]) && isprint(d->inbuf[i]))
                            d->incomm[k++] = d->inbuf[i];
                            /*
                             * handling vt100 feedback 
                             */
                            else
                            if (d->inbuf[i] == '\e')
                            {
                            char xbuf[30];  // plenty
                            char *xptr;
                            int xk = 1; xbuf[0] = '\0'; xptr = xbuf;
                            /*
                             * first we scan to see if this is really a reply to our request 
                             */
                            while (d->inbuf[i + xk] != 'R' && xk < 10) xk++;
                            /*
                             * I'm sure most screens use at most 999x999 chars 
                             */
                            if (xk < 10)
                            {
                            i += 2; xk = 0;
                            while (d->inbuf[i] != ';' && d->inbuf[i] != '\0'
                                   && ++xk < 30)
                            {
                            *xptr = d->inbuf[i]; xptr++; i++;}
                            *xptr = '\0'; init_vt100(d, xbuf);}
                            }
                            else
                            if (d->inbuf[i] == (signed char) IAC)
                            {
                            if (!memcmp
                                (&d->inbuf[i], compress_do,
                                 strlen(compress_do)))
                            {
                            i += strlen(compress_do) - 1; compressStart(d);}
                            else
                            if (!memcmp
                                (&d->inbuf[i], compress_dont,
                                 strlen(compress_dont)))
                            {
                            i += strlen(compress_dont) - 1; compressEnd(d);}
                            }

                            }

                            /*
                             * Finish off the line.
                             */
                            if (k == 0)
                            d->incomm[k++] = ' '; d->incomm[k] = '\0';
                            /*
                             * Deal with bozos with #repeat 1000 ...
                             */
                            if (k > 1 || d->incomm[0] == '!')
                            {
                            if (d->incomm[0] != '!'
                                && strcmp(d->incomm, d->inlast))
                            {
                            d->repeat = 0;}
                            else
                            {
                            if (++d->repeat >= 40)
                            {
                            if (d != NULL && d->character != NULL)
                            {
                            xprintf(buf, "%s input overflow!",
                                    d->character->lasthost);
                            wiznet(buf, NULL, NULL, WIZ_DEBUG, 0, 0);
                            log_string(LOG_COMMAND, buf);}
                            else
                            if (d->lookup_status != STATUS_LOOKUP)
                            {
                            xprintf(buf, "%s input overflow!", d->host);
                            wiznet(buf, NULL, NULL, WIZ_DEBUG, 0, 0);
                            log_string(LOG_COMMAND, buf);}
                            write_to_descriptor(d,
                                                "\n\r*** PUT A LID ON IT!!! ***\n\r",
                                                0);
                            xprintf_2(d->incomm, "quit");}
                            }
                            }

                            /*
                             * Do '!' substitution.
                             */
                            if (d->incomm[0] == '!')
                            strcpy(d->incomm, d->inlast);
                            else
                            strcpy(d->inlast, d->incomm);
                            /*
                             * Shift the input buffer.
                             */
                            while (d->inbuf[i] == '\n' || d->inbuf[i] == '\r')
                            i++;
                            for (j = 0;
                                 (d->inbuf[j] = d->inbuf[i + j]) != '\0';
                                 j++); return;}



#define COPYOVER_FILE "copyover.data"
#define EXE_FILE      "../src/mcloud"

                            void panic_shutdown(int iSignal)
                            {
                            DESCRIPTOR_DATA * d;
                            CHAR_DATA * gch;
                            FILE * fp;
                            char buf[200];
                            xprintf_2(buf,
                                      "\n\rServer reboot, saving and doing a panic shutdown.");
                            /*
                             * We really don't want to lose this info 
                             */
                            save_coreinfo();
                            /*
                             * save all connected players 
                             */
                            for (d = descriptor_list; d; d = d->next)
                            {
                            if ((gch = d->character) == NULL) continue;
                            call_all(gch);
                            save_char_obj(gch);
                            write_to_descriptor_2(d->descriptor, buf, 0);}

                            /*
                             * create a bug message so it's easy to know what happended 
                             */
                            bug("Server Rebooted, handling it safely.", 0);
                            /*
                             * shutdown file - probably not needed, but hey 
                             */
                            if ((fp = fopen(SHUTDOWN_FILE, "w")) != NULL)
                            {
                            fprintf(fp, "Server Reboot\n"); fclose(fp);}

                            /*
                             * and we die 
                             */
                            exit(0);}

/*
 * To do internal backtracs at crashes
 */

                            void iBacktrace(int iSignal)
                            {
                            void *aTrace[10]; char **tString; char buf[400];    //plenty
                            int size, i, j, k;
                            size = backtrace(aTrace, 10);
                            printf("%s\n", (char *) aTrace[1]);
                            tString = backtrace_symbols(aTrace, size);
                            for (i = 0; i < size; i++)
                            {
                            bool found = FALSE;
                            printf("%s\n", tString[i]);
                            for (j = 0; tString[i][j] != '\0' && !found; j++)
                            {
                            if (tString[i][j] == '[')
                            {
                            j++;
                            found = TRUE;
                            for (k = j;
                                 tString[i][j] != '\0'
                                 && tString[i][j] != ']'; j++)
                            {
                            buf[j - k] = tString[i][j];}
                            buf[j - k] = '\0';}
                            }
                            printf("%s translates to : %s\n", buf,
                                   (char *) strtol(buf, NULL, 16));}

                            exit(1);}

/*
 * Crash recovery system written by Mandrax, based on copyover
 * Includes call to signal() in main.
 * Mostly copied from copyover.
 */
                            void crashrecov(int iSignal)
                            {
                            CHAR_DATA * gch;
                            OBJ_DATA * obj; FILE * fp; DESCRIPTOR_DATA * d;
                            //DESCRIPTOR_DATA *d_next;
                            char buf[200], buf2[100];
                            int pid;
                            int iFork; int i; FILE * fReturn; FILE * fCrash;
                            /*
                             * unlock memory mutex - prevent lockups 
                             */
                            pthread_mutex_unlock(&memory_mutex);
                            /*
                             * An attempt to avoid spam crashes
                             */
                            if ((fReturn =
                                 fopen(CRASH_TEMP_FILE, "r")) != NULL)
                            {
                            signal(SIGSEGV, SIG_IGN); raise(SIGSEGV); return;}
                            fCrash = fopen(CRASH_TEMP_FILE, "w");
                            fprintf(fCrash, "0"); fclose(fCrash);
                            //save_polls();
                            dump_last_command();
                            /*
                             * This will cause a core dump, even though the signal was handled
                             */
                            iFork = fork();
                            wait(NULL); if ((pid = getpid()) != proc_pid)
                            {
                            signal(SIGSEGV, SIG_IGN); raise(SIGSEGV); return;}

                            fp = fopen(COPYOVER_FILE, "w"); if (!fp)
                            {
                            perror("crashrecov:fopen"); return;}

                            /*
                             * save and stuff 
                             */
                            for (gch = char_list; gch != NULL;
                                 gch = gch->next)
                            {
                            if (IS_NPC(gch)) continue;
                            /*
                             * Fix any possibly head/object forms 
                             */
                            if (IS_HEAD(gch, LOST_HEAD)
                                || IS_SET(gch->extra, EXTRA_OSWITCH))
                            {
                            REMOVE_BIT(gch->loc_hp[0], LOST_HEAD);
                            REMOVE_BIT(gch->affected_by, AFF_POLYMORPH);
                            REMOVE_BIT(gch->extra, EXTRA_OSWITCH);
                            gch->morph = str_dup("");
                            gch->pcdata->chobj = NULL;
                            gch->pcdata->obj_vnum = 0;
                            char_from_room(gch);
                            char_to_room(gch,
                                         get_room_index(ROOM_VNUM_ALTAR));}

                            /*
                             * Take care of possibly artifacts 
                             */
                            for (i = 0; i < MAX_WEAR; i++)
                            {
                            if ((obj = get_eq_char(gch, i)) != NULL)
                            {
                            if (IS_SET(obj->quest, QUEST_ARTIFACT))
                            unequip_char(gch, obj);}
                            }

                            gch->hit = gch->max_hit;
                            gch->position = POS_STANDING;
                            do_call(gch, "all"); save_char_obj(gch);}

                            xprintf_2(buf,
                                      "\n\r <*>          Mindcloud 2 has crashed        <*>\n\r\n\r"
                                      " <*>   Attempting to restore last savefile   <*>\n\r");
                            /*
                             * For each playing descriptor, save its state 
                             */
                            for (d = descriptor_list; d; d = d_next)
                            {
                            CHAR_DATA * och = d->character; compressEnd2(d); d_next = d->next;  /* We delete from the list , so need to save this */
                            if (!d->character || d->connected != 0) /* drop those logging on */
                            {
                            write_to_descriptor_2(d->descriptor, "\n\rSorry, the mud has crashed.\n\rPlease log on another char and report this, your char MAY be bugged.\n\r", 0); close_socket2(d, FALSE);    /* throw'em out */
                            }
                            else
                            {
                            fprintf(fp, "%d %s %s\n", d->descriptor,
                                    och->name, d->host);
                            write_to_descriptor_2(d->descriptor, buf, 0);}
                            }
                            fprintf(fp, "-1\n");
                            fclose(fp); fclose(fpReserve);
                            /*
                             * recycle descriptors 
                             */
                            recycle_descriptors();
                            /*
                             * exec - descriptors are inherited 
                             */
                            xprintf(buf, "%d", port);
                            xprintf(buf2, "%d", control);
                            execl(EXE_FILE, "mcloud", buf, "crashrecov", buf2,
                                  (char *) NULL);
                            /*
                             * Failed - sucessful exec will not return 
                             */
                            perror("crashrecov: execl");
                            log_string(LOG_ERR, "Crash recovery FAILED!\n\r");
                            wiznet("Crash recovery FAILED!\n\r", NULL, NULL,
                                   WIZ_DEBUG, 0, 0);
                            /*
                             * The least we can do is exit gracefully :P 
                             */
                            exit(1);}

                            void retell_mccp(DESCRIPTOR_DATA * d)
                            {
                            write_to_buffer(d, compress_will, 0); return;}


                            bool process_output(DESCRIPTOR_DATA * d,
                                                bool fPrompt)
                            {
                            //extern bool merc_down;

                            /*
                             * Bust a prompt.
                             */
                            //if ( d->showstr_point )
                            //      write_to_buffer ( d, "[Hit Return to continue]\n\r", 0 );

                            if (d->showstr_point)
                            {
                            char buf[MSL];
                            const char *ptr;
                            int shown = 0;
                            int total = 0;
                            for (ptr = d->showstr_head;
                                 ptr != d->showstr_point;
                                 ptr++) if (*ptr == '\n') shown++;
                            total = shown; for (ptr = d->showstr_point;
                                                *ptr != '\0';
                                                ptr++) if (*ptr ==
                                                           '\n') total++;
                            xprintf(buf,
                                    "\n\rPlease type (R)efresh, (H)elp, (B)ack or (C)ontinue or ENTER (%d%%)\n\r",
                                    100 * shown / total);
                            write_to_buffer(d, buf, 0);}

                            if (fPrompt && !merc_down
                                && (d->connected == CON_PLAYING
                                    || d->connected == CON_PFILE))
                            {
                            char buf[MSL];  //more than plenty
                            CHAR_DATA * ch;
                            CHAR_DATA * victim;
                            ch = d->original ? d->original : d->character;
                            if (IS_SET(ch->act, PLR_BRIEF5))
                            {

                            if (ch->amount_attacks_dealt > 0)
                            {
                            xprintf(buf,
                                    "#wYou've dealt #Y%d #wattacks causing #R%d #wdamage. #0(#Y%d #Gaverage#0)#n\n\r",
                                    ch->amount_attacks_dealt,
                                    ch->amount_damage_dealt,
                                    ch->amount_damage_dealt /
                                    ch->amount_attacks_dealt);
                            send_to_char(buf, ch);}
                            if (ch->amount_attacks_recieved > 0)
                            {
                            xprintf(buf,
                                    "#wYou've been struck by #Y%d #wattacks, causing #R%d #wdamage. #0(#Y%d #Gaverage#0)#n\n\r",
                                    ch->amount_attacks_recieved,
                                    ch->amount_damage_recieved,
                                    ch->amount_damage_recieved /
                                    ch->amount_attacks_recieved);
                            send_to_char(buf, ch);}
                            if ((ch->total_exp_gained > 0)
                                && (ch->total_kills > 0))
                            {
                            xprintf(buf,"#wYou've killed #Y%d #wthings and gained a total of #R%d#w exp#n\n\r",
                                    ch->total_kills, ch->total_exp_gained);
                            send_to_char(buf, ch);}
                            else
                            if (ch->total_exp_gained > 0)
                            {
                            xprintf(buf,
                                    "#wYou've gained a total of #Y%d #wexp#n\n\r",
                                    ch->total_exp_gained);
                            send_to_char(buf, ch);}
                            if (ch->total_disc_gained > 0)
                            {
                            xprintf(buf,
                                    "#wYou've gained a total of #G%d #wdiscipline points#n\n\r",
                                    ch->total_disc_gained);
                            send_to_char(buf, ch);}
                            if (ch->total_cp_gained > 0)
                            {
                            xprintf(buf,
                                    "#wYou've gained a total of #P%d #wclass points#n\n\r",
                                    ch->total_cp_gained);
                            send_to_char(buf, ch);}

                            ch->amount_damage_dealt = 0;
                            ch->amount_damage_recieved = 0;
                            ch->amount_attacks_dealt = 0;
                            ch->amount_attacks_recieved = 0;
                            ch->total_exp_gained = 0;
                            ch->total_disc_gained = 0;
                            ch->total_kills = 0; ch->total_cp_gained = 0;}

                            if (!IS_NPC(ch)
                                && IS_SET(ch->pcdata->tempflag, TEMP_VT100))
                            {
                            xprintf(buf, "%s\e[%d;1H", VT_SAVECURSOR,
                                    ch->pcdata->vt100_size);
                            write_to_buffer(d, buf, 0);}
                            write_to_buffer(d, "\n\r", 2);
                            if (IS_SET(ch->act, PLR_PROMPT)
                                && IS_EXTRA(ch,
                                            EXTRA_PROMPT)) bust_a_prompt(d);
                            else
                            if (IS_SET(ch->act, PLR_PROMPT))
                            {
                            //char buf[MAX_STRING_LENGTH];
                            char cond[MAX_INPUT_LENGTH];
                            char hit_str[MAX_INPUT_LENGTH];
                            char mana_str[MAX_INPUT_LENGTH];
                            char move_str[MAX_INPUT_LENGTH];
                            char exp_str[MAX_INPUT_LENGTH];
                            ch = d->character;
                            if (IS_HEAD(ch, LOST_HEAD)
                                || IS_EXTRA(ch, EXTRA_OSWITCH))
                            {
        		    xprintf(exp_str, "%2.0f", ch->exp);
                            xprintf(buf,
                                    "#n<[#C%sX#n] [#C?#bH #C?#bM #C?#bV#n]> ",
                                    exp_str);}
                            else
                            if (ch->position == POS_FIGHTING)
                            {
                            victim = ch->fighting; if (victim == NULL)
                            {
                            xprintf(cond, "NA"); ADD_COLOUR(ch, cond, L_RED);}
                            if ((victim->hit * 100 / victim->max_hit) < 25)
                            {
                            xprintf(cond, "#0[#R*    #0]#n");}
                            else
                            if ((victim->hit * 100 / victim->max_hit) < 50)
                            {
                            xprintf(cond, "#0[#r**   #0]#n");}
                            else
                            if ((victim->hit * 100 / victim->max_hit) < 75)
                            {
                            xprintf(cond, "#0[#Y***  #0]#n");}
                            else
                            if ((victim->hit * 100 / victim->max_hit) < 100)
                            {
                            xprintf(cond, "#0[#g**** #0]#n");}
                            else
                            if ((victim->hit * 100 / victim->max_hit) >= 100)
                            {
                            xprintf(cond, "#0[#G*****#0]#n");}
                            xprintf(hit_str, "%s", commanum(ch->hit));
                            COL_SCALE(hit_str, ch, ch->hit, ch->max_hit);
                            xprintf(mana_str, "%s", commanum(ch->mana));
                            COL_SCALE(mana_str, ch, ch->mana, ch->max_mana);
                            xprintf(move_str, "%s", commanum(ch->move));
                            COL_SCALE(move_str, ch, ch->move, ch->max_move);
                            xprintf(buf,
                                    "#0-=#n%s#0=- -=#n%s#Rhp #n%s#Rm #n%s#Rmv#0=- #n",
                                    cond, hit_str, mana_str, move_str);}
                            else
                            {
                            xprintf(hit_str, "%s", commanum(ch->hit));
                            COL_SCALE(hit_str, ch, ch->hit, ch->max_hit);
                            xprintf(mana_str, "%s", commanum(ch->mana));
                            COL_SCALE(mana_str, ch, ch->mana, ch->max_mana);
                            xprintf(move_str, "%s", commanum(ch->move));
                            COL_SCALE(move_str, ch, ch->move, ch->max_move);
		            xprintf(exp_str, "%2.0f", ch->exp);
                            xprintf(buf,
                                    "#0-=#n%s#Rxp#0=- -=#n%s#Rhp #n%s#Rm #n%s#Rmv#0=- #n",
                                    exp_str, hit_str, mana_str, move_str);}
                            write_to_buffer(d, buf, 0);}

                            if (!IS_NPC(ch)
                                && IS_SET(ch->pcdata->tempflag, TEMP_VT100))
                            {
                            write_to_buffer(d, VT_RESTORECURSOR, 0);}

                            if (IS_SET(ch->act, PLR_TELNET_GA))
                            write_to_buffer(d, go_ahead_str, 0);}

                            /*
                             * Short-circuit if nothing to write.
                             */
                            if (d->outtop == 0) return TRUE;
                            /*
                             * Snoop-o-rama.
                             */
                            if (d->snoop_by != NULL)
                            {
                            write_to_buffer(d->snoop_by, "% ", 2);
                            write_to_buffer(d->snoop_by, d->outbuf,
                                            d->outtop);}

                            /*
                             * OS-dependent output.
                             */
                            if (!write_to_descriptor(d, d->outbuf, d->outtop))
                            {
                            d->outtop = 0; return FALSE;}
                            else
                            {
                            d->outtop = 0; return TRUE;}
                            }

                            void write_to_buffer(DESCRIPTOR_DATA * d,
                                                 const char *txt, int length)
                            {
                            char tb[32000], ccode;
                            int i, j;
                            bool ansi;
                            if (length <= 0)
                            length = strlen(txt);
                            if (length >= MAX_STRING_LENGTH)
                            {
                            d->connected = CON_PLAYING;
                            d->outsize = 2000;
                            d->outbuf = alloc_mem(d->outsize); d->outtop = 2;
                            //send_to_char("#1Too much text to show, clearing...#n",d->character);
                            //logf("Too much text to show, clearing..... (%s)",d->character->name);
                            return;}

                            if (d->character == NULL ||
                                (d->connected != CON_PLAYING)) ansi = FALSE;
                            else
                            ansi =
                            (IS_SET(d->character->act, PLR_ANSI)) ? TRUE :
                            FALSE;
                            if (d->connected == CON_GET_NAME) ansi = TRUE;
                            if (d->outtop == 0 && !d->fcommand)
                            {
                            d->outbuf[0] = '\n';
                            d->outbuf[1] = '\r'; d->outtop = 2;}

                            j = 0;
                            ccode = '3';
                            tb[0] = '\0'; for (i = 0; i < length; i++)
                            {
                            if (txt[i] == '#' && txt[i + 1] != 'I'
                                && txt[i + 1] != 'N')
                            {
                            i++; ccode = '3'; switch (txt[i])
                            {
default:
tb[j++] = '#'; tb[j++] = txt[i]; break; case '-':
                            tb[j++] = '~'; break;
                            //case '+': tb[j++] = '%'; break;
case '#':
tb[j++] = '#'; break; case 'I':
case 'i':
tb[j++] = 27; tb[j++] = '['; tb[j++] = '7'; tb[j++] = 'm'; break; case 'f':
tb[j++] = 27; tb[j++] = '['; tb[j++] = '5'; tb[j++] = 'm'; break; case 'u':
tb[j++] = 27; tb[j++] = '['; tb[j++] = '4'; tb[j++] = 'm'; break; case 'd':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '5'; tb[j++] = '1'; break; case 'N':
case 'n':
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = 'm'; break; case 'B':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm'; break; case 'L':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm'; break; case '0':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '0'; tb[j++] = 'm'; break; case 'D':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '0'; tb[j++] = 'm'; break; case '1':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm'; break; case '2':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm'; break; case '3':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm'; break; case '4':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm'; break; case '5':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm'; break; case '6':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm'; break; case '7':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm'; break; case '8':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm'; break; case 'w':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm'; break; case 'W':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm'; break; case '9':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '7'; tb[j++] = ';'; tb[j++] = '4'; tb[j++] = '1'; tb[j++] = 'm'; break; case 'r':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm'; break; case 'R':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm'; break; case 'g':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm'; break; case 'G':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm'; break; case 'o':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm'; break; case 'y':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm'; break; case 'Y':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm'; break; case 'l':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm'; break; case 'b':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm'; break; case 'p':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm'; break; case 'P':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm'; break; case 'c':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm'; break; case 'C':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm'; break; case 'e':
                            if (!ansi) continue; switch (number_range(1, 3))
                            {
case 1:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm'; break; case 2:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm'; break; case 3:
                            tb[j++] = 27; tb[j++] = '['; tb[j++] = '1';
                            tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '7';
                            tb[j++] = 'm'; break;}
break; case 'k':
                            if (!ansi) continue; switch (number_range(1, 6))
                            {
case 1:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm'; break; case 2:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm'; break; case 3:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm'; break; case 4:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm'; break; case 5:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm'; break; case 6:
                            tb[j++] = 27; tb[j++] = '['; tb[j++] = '1';
                            tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '0';
                            tb[j++] = 'm'; break;}
break; case 'q':
                            if (!ansi) continue; switch (number_range(1, 6))
                            {
case 1:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm'; break; case 2:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm'; break; case 3:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm'; break; case 4:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm'; break; case 5:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm'; break; case 6:
                            tb[j++] = 27; tb[j++] = '['; tb[j++] = '1';
                            tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '0';
                            tb[j++] = 'm'; break;}
break; case 'a':
                            if (!ansi) continue; switch (number_range(1, 6))
                            {
case 1:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm'; break; case 2:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm'; break; case 3:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm'; break; case 4:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm'; break; case 5:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm'; break; case 6:
                            tb[j++] = 27; tb[j++] = '['; tb[j++] = '1';
                            tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '0';
                            tb[j++] = 'm'; break;}
break; case 'h':
                            if (!ansi) continue; switch (number_range(1, 6))
                            {
case 1:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm'; break; case 2:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm'; break; case 3:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm'; break; case 4:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm'; break; case 5:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm'; break; case 6:
                            tb[j++] = 27; tb[j++] = '['; tb[j++] = '1';
                            tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '0';
                            tb[j++] = 'm'; break;}
break; case 'x':
                            if (!ansi) continue; switch (number_range(1, 8))
                            {
case 1:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm'; break; case 2:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm'; break; case 3:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm'; break; case 4:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm'; break; case 5:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm'; break; case 6:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm'; break; case 7:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm'; break; case 8:
                            tb[j++] = 27; tb[j++] = '['; tb[j++] = '0';
                            tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '4';
                            tb[j++] = 'm'; break;}
break; case '*':
                            switch (number_range(1, 3))
                            {
case 1:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm'; break; case 2:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm'; break; case 3:
                            tb[j++] = 27; tb[j++] = '['; tb[j++] = '0';
                            tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '6';
                            tb[j++] = 'm'; break;}
break; case '+':
                            if (!ansi) continue; switch (number_range(1, 6))
                            {
case 1:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '0'; tb[j++] = 'm'; break; case 2:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '0'; tb[j++] = 'm'; break; case 3:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm'; break; case 4:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm'; break; case 5:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm'; break; case 6:
                            tb[j++] = 27; tb[j++] = '['; tb[j++] = '1';
                            tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '0';
                            tb[j++] = 'm'; break;;}
break; case 'v':
                            if (!ansi) continue; switch (number_range(1, 6))
                            {
case 1:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm'; break; case 2:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm'; break; case 3:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm'; break; case 4:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm'; break; case 5:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm'; break; case 6:
                            tb[j++] = 27; tb[j++] = '['; tb[j++] = '1';
                            tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '0';
                            tb[j++] = 'm'; break;}
break; case 'z':
                            switch (number_range(1, 6))
                            {
case 1:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm'; break; case 2:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm'; break; case 3:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm'; break; case 4:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm'; break; case 5:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm'; break; case 6:
                            tb[j++] = 27; tb[j++] = '['; tb[j++] = '1';
                            tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '7';
                            tb[j++] = 'm'; break;}
break; case 't':
                            if (!ansi) continue; switch (number_range(1, 6))
                            {
case 1:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm'; break; case 2:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm'; break; case 3:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm'; break; case 4:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm'; break; case 5:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm'; break; case 6:
                            tb[j++] = 27; tb[j++] = '['; tb[j++] = '0';
                            tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '3';
                            tb[j++] = 'm'; break;}
break; case 's':
                            if (!ansi) continue; switch (number_range(1, 15))
                            {
case 1:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm'; break; case 2:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm'; break; case 3:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm'; break; case 4:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '0'; tb[j++] = 'm'; break; case 5:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm'; break; case 6:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm'; break; case 7:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm'; break; case W:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm'; break; case 9:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm'; break; case 10:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm'; break; case 11:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm'; break; case 12:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm'; break; case 13:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm'; break; case 14:
tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm'; break; case 15:
                            tb[j++] = 27; tb[j++] = '['; tb[j++] = '0';
                            tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '6';
                            tb[j++] = 'm'; break;}
                            break;}
                            }
                            else
                            if (txt[i] == '{' && txt[i + 1] != '#')
                            {
                            i++; ccode = '4'; switch (txt[i])
                            {
default:
tb[j++] = '{'; tb[j++] = txt[i]; break; case '{':
tb[j++] = txt[i]; break; case 'N':
case 'n':
tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm'; tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = 'm'; break; case '0':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '0'; tb[j++] = 'm'; break; case '1':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm'; break; case '2':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm'; break; case '3':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm'; break; case '4':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm'; break; case '5':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm'; break; case '6':
if (!ansi) continue; tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm'; break; case '7':
                            if (!ansi) continue;
                            tb[j++] = 27; tb[j++] = '['; tb[j++] = '1';
                            tb[j++] = ';'; tb[j++] = ccode; tb[j++] = '7';
                            tb[j++] = 'm'; break;}
                            }
                            else
                            tb[j++] = txt[i];}

                            tb[j] = '\0';
                            length = j; if (length >= MAX_STRING_LENGTH)
                            {
                            d->connected = CON_PLAYING;
                            d->outsize = 2000;
                            d->outbuf = alloc_mem(d->outsize);
                            d->outtop = 2;
                            xprintf(tb,
                                    "Too much text to show, clearing....");
                            length = strlen(tb);
                            //logf("Too much text to show, clearing..... (%s)",d->character->name);
                            }

                            if (d->outtop + length >= 32000)
                            {
                            d->connected = CON_PLAYING;
                            d->outsize = 2000;
                            d->outbuf = alloc_mem(d->outsize);
                            d->outtop = 2;
                            xprintf(tb,
                                    "Too much text to show, clearing....");
                            length = strlen(tb);
                            //logf("Too much text to show, clearing..... (%s)",d->character->name);
                            }

                            while (d->outtop + length >= d->outsize)
                            {
                            char *outbuf;
                            outbuf = alloc_mem(2 * d->outsize);
                            strncpy(outbuf, d->outbuf, d->outtop);
                            free_mem(d->outbuf, d->outsize);
                            d->outbuf = outbuf; d->outsize *= 2;}

                            /*
                             * Copy.
                             */
                            strncpy(d->outbuf + d->outtop, tb, length);
                            d->outtop += length; return;}

/*
void write_to_buffer( DESCRIPTOR_DATA *d, const char *txt, int length)
{
  static char output[32000];
  char *ptr;
  int i = 0;

  output[0] = '\0';
  ptr = output;

  if (length <= 0)
    length = strlen(txt);

  //if (length >= MAX_STRING_LENGTH )
  //{
  //  bug( "Write_to_buffer: Way too big. Closing.", 0 );
  //  return;
  //}
  
  if ( length >= MAX_STRING_LENGTH )
    {
            d->connected        = CON_PLAYING;
            d->outsize          = 2000;
            d->outbuf           = alloc_mem( d->outsize );
            d->outtop       = 2;
	    send_to_char("#1Too much text to show, clearing...#n",d->character);
            logf("Too much text to show, clearing..... (%s)",d->character->name);
        return;
    }

  // initial linebreak
  if ( d->outtop == 0 && !d->fcommand )
  {
    d->outbuf[0]    = '\n';
    d->outbuf[1]    = '\r';
    d->outtop       = 2;
  }
  
  if (d->character == NULL ||
        (d->connected != CON_PLAYING))
        ansi = FALSE;
    else ansi = (IS_SET(d->character->act, PLR_ANSI)) ? TRUE : FALSE;
    if (d->connected == CON_GET_NAME) ansi = TRUE;

  while (*txt != '\0' && i++ < length)
  {
    switch(*txt)
    {
      default:
        *ptr++ = *txt++;
        break;
      case '#':
        i++;
        switch (*++txt)
        {
          default:
            txt++;
            break;
          case '#':
            *ptr++ = '#';
            txt++;
            break;
          case '-':
            *ptr++ = '~';
            txt++;
            break;
          case '+':
            *ptr++ = '%';
            txt++;
            break;
          case 'i':  // bold
            *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '7';
            *ptr++ = 'm';   txt++;          break;
          case 'f':  // flash
            *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '5';
            *ptr++ = 'm';   txt++;          break;
          case 'u':  // underline
            *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '4';
            *ptr++ = 'm';   txt++;          break;
          case 'n':  // stock color
            *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
            *ptr++ = 'm';   txt++;          break;
          case 'w':
          case '7':  // White
            *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
            *ptr++ = ';';   *ptr++ = '0';   *ptr++ = ';';
            *ptr++ = '3';   *ptr++ = '7';   *ptr++ = 'm';
            txt++;          break;
          case '8':  // Black
            *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
            *ptr++ = ';';   *ptr++ = '0';   *ptr++ = ';';
            *ptr++ = '3';   *ptr++ = '0';   *ptr++ = 'm';
            txt++;          break;
          case 'W':
          case '9':  // White 2
            *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
            *ptr++ = ';';   *ptr++ = '1';   *ptr++ = ';';
            *ptr++ = '3';   *ptr++ = '7';   *ptr++ = 'm';
            txt++;          break;
          case 'D':
          case '0':  // Black 2
            *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
            *ptr++ = ';';   *ptr++ = '1';   *ptr++ = ';';
            *ptr++ = '3';   *ptr++ = '0';   *ptr++ = 'm';
            txt++;          break;
          case 'r':  // Red
            *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
            *ptr++ = ';';   *ptr++ = '0';   *ptr++ = ';';
            *ptr++ = '3';   *ptr++ = '1';   *ptr++ = 'm';
            txt++;          break;
          case '1':
          case 'R':  // red 2
            *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
            *ptr++ = ';';   *ptr++ = '1';   *ptr++ = ';';
            *ptr++ = '3';   *ptr++ = '1';   *ptr++ = 'm';
            txt++;          break;
          case 'g': // Green
            *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
            *ptr++ = ';';   *ptr++ = '0';   *ptr++ = ';';
            *ptr++ = '3';   *ptr++ = '2';   *ptr++ = 'm';
            txt++;          break;
          case '2':
          case 'G': // Green 2
            *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
            *ptr++ = ';';   *ptr++ = '1';   *ptr++ = ';';
            *ptr++ = '3';   *ptr++ = '2';   *ptr++ = 'm';
            txt++;          break;
          case 'y':
          case 'o':  // Yellow (Brown)
            *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
            *ptr++ = ';';   *ptr++ = '0';   *ptr++ = ';';
            *ptr++ = '3';   *ptr++ = '3';   *ptr++ = 'm';
            txt++;          break;
          case 'Y':
            *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
            *ptr++ = ';';   *ptr++ = '1';   *ptr++ = ';';
            *ptr++ = '3';   *ptr++ = '3';   *ptr++ = 'm';
            txt++;          break;
          case 'b':
          case 'l':  // Blue
            *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
            *ptr++ = ';';   *ptr++ = '0';   *ptr++ = ';';
            *ptr++ = '3';   *ptr++ = '4';   *ptr++ = 'm';
            txt++;          break;
          case '4':
          case 'B':
          case 'L':  // Blue 2
            *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
            *ptr++ = ';';   *ptr++ = '1';   *ptr++ = ';';
            *ptr++ = '3';   *ptr++ = '4';   *ptr++ = 'm';
            txt++;          break;
          case 'p':  // Purple
            *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
            *ptr++ = ';';   *ptr++ = '0';   *ptr++ = ';';
            *ptr++ = '3';   *ptr++ = '5';   *ptr++ = 'm';
            txt++;          break;
          case '5':
          case 'P':  // Purple 2
            *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
            *ptr++ = ';';   *ptr++ = '1';   *ptr++ = ';';
            *ptr++ = '3';   *ptr++ = '5';   *ptr++ = 'm';
            txt++;          break;
          case 'c':  // Cyan
            *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
            *ptr++ = ';';   *ptr++ = '0';   *ptr++ = ';';
            *ptr++ = '3';   *ptr++ = '6';   *ptr++ = 'm';
            txt++;          break;
          case '6':
          case 'C':  // Cyan 2
            *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
            *ptr++ = ';';   *ptr++ = '1';   *ptr++ = ';';
            *ptr++ = '3';   *ptr++ = '6';   *ptr++ = 'm';
            txt++;          break;
          case 's':
            switch(number_range(1,15))                                
            {
              case 1:  // White
                *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
                *ptr++ = ';';   *ptr++ = '1';   *ptr++ = ';';
                *ptr++ = '3';   *ptr++ = '7';   *ptr++ = 'm';
                break;
              case 2:  // Black
                *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
                *ptr++ = ';';   *ptr++ = '1';   *ptr++ = ';';
                *ptr++ = '3';   *ptr++ = '0';   *ptr++ = 'm';
                break;
              case 3:  // Black 2
              case 4:  // Red
                *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
                *ptr++ = ';';   *ptr++ = '0';   *ptr++ = ';';
                *ptr++ = '3';   *ptr++ = '1';   *ptr++ = 'm';
                break;
              case 5:  // red 2
                *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
                *ptr++ = ';';   *ptr++ = '1';   *ptr++ = ';';
                *ptr++ = '3';   *ptr++ = '1';   *ptr++ = 'm';
                break;
              case 6: // Green
                *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
                *ptr++ = ';';   *ptr++ = '0';   *ptr++ = ';';
                *ptr++ = '3';   *ptr++ = '2';   *ptr++ = 'm';
                break;
              case 7: // Green 2
                *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
                *ptr++ = ';';   *ptr++ = '1';   *ptr++ = ';';
                *ptr++ = '3';   *ptr++ = '2';   *ptr++ = 'm';
                break;
              case 8:  // Yellow (Brown)
                *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
                *ptr++ = ';';   *ptr++ = '0';   *ptr++ = ';';
                *ptr++ = '3';   *ptr++ = '3';   *ptr++ = 'm';
                break;
              case 9:  // Yellow 2
                *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
                *ptr++ = ';';   *ptr++ = '1';   *ptr++ = ';';
                *ptr++ = '3';   *ptr++ = '3';   *ptr++ = 'm';
                break;
              case 10:  // Blue
                *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
                *ptr++ = ';';   *ptr++ = '0';   *ptr++ = ';';
                *ptr++ = '3';   *ptr++ = '4';   *ptr++ = 'm';
                break;
              case 11:  // Blue 2
                *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
                *ptr++ = ';';   *ptr++ = '1';   *ptr++ = ';';
                *ptr++ = '3';   *ptr++ = '4';   *ptr++ = 'm';
                break;
              case 12:  // Purple
                *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
                *ptr++ = ';';   *ptr++ = '0';   *ptr++ = ';';
                *ptr++ = '3';   *ptr++ = '5';   *ptr++ = 'm';
                break;
              case 13:  // Purple 2
                *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
                *ptr++ = ';';   *ptr++ = '1';   *ptr++ = ';';
                *ptr++ = '3';   *ptr++ = '5';   *ptr++ = 'm';
                break;
              case 14:  // Cyan
                *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
                *ptr++ = ';';   *ptr++ = '0';   *ptr++ = ';';
                *ptr++ = '3';   *ptr++ = '6';   *ptr++ = 'm';
              case 15:  // Cyan 2
                *ptr++ = '\e';  *ptr++ = '[';   *ptr++ = '0';
                *ptr++ = ';';   *ptr++ = '1';   *ptr++ = ';';
                *ptr++ = '3';   *ptr++ = '6';   *ptr++ = 'm';
                break;
            }
            txt++;
            break;
        }
    }
  }

  // and terminate it with the standard color
  *ptr++ = '\e';  *ptr++ = '[';
  *ptr++ = '0';   *ptr++ = 'm';

  // terminate
  *ptr = '\0';

  length = strlen(output);


    if ( length >= MAX_STRING_LENGTH )
    {
            d->connected        = CON_PLAYING;
            d->outsize          = 2000;
            d->outbuf           = alloc_mem( d->outsize );   
            d->outtop       = 2;
            xprintf(output,"Too much text to show, clearing....");
            length=strlen(output);
            logf("Too much text to show, clearing..... (%s)",d->character->name);
    }

        if (d->outtop + length >= 32000)
        {
            d->connected        = CON_PLAYING;
            d->outsize          = 2000;
            d->outbuf           = alloc_mem( d->outsize );   
            d->outtop       = 2;
	    xprintf(output,"Too much text to show, clearing....");
	    length=strlen(output);
	    logf("Too much text to show, clearing..... (%s)",d->character->name);
        }
  // Expand the buffer as needed.
  while (d->outtop + length >= d->outsize)
  {
    char *obuf;

    // There is a limit to our patience
    //if (d->outsize >= 32000)
    //{
    //  bug("Buffer overflow. Closing.",0);
    //  close_socket(d);
    //  return;
    //}
    obuf = alloc_mem(2 * d->outsize);
    strncpy(obuf, d->outbuf, d->outtop);
    free_mem(d->outbuf, d->outsize);
    d->outbuf = obuf;
    d->outsize *= 2;
  }

  // Copy.
  strncpy(d->outbuf + d->outtop, output, length);
  d->outtop += length;
  return;
}
*/

/*
 * Lowest level output function.
 * Write a block of text to the file descriptor.
 * If this gives errors on very long blocks (like 'ofind all'),
 *   try lowering the max block size.
 */
                            bool write_to_descriptor_2(int desc, char *txt,
                                                       int length)
                            {
                            int iStart; int nWrite; int nBlock;
#if defined(macintosh) || defined(MSDOS)
                            if (desc == 0) desc = 1;
#endif
                            if (length <= 0)
                            length = strlen(txt);
                            increase_total_output(length);
                            for (iStart = 0; iStart < length;
                                 iStart += nWrite)
                            {
                            nBlock = UMIN(length - iStart, 4096);
#if !defined(WIN32)
                            if ((nWrite =
                                 write(desc, txt + iStart, nBlock)) < 0)
#else
                            if ((nWrite =
                                 send(desc, txt + iStart, nBlock, 0)) < 0)
#endif
                            {
                            perror("Write_to_descriptor"); return FALSE;}
                            }

                            return TRUE;}

/* mccp: write_to_descriptor wrapper */
                            bool write_to_descriptor(DESCRIPTOR_DATA * d,
                                                     char *txt, int length)
                            {
                            if (d->out_compress)
                            return writeCompressed(d, txt, length);
                            else
                            return write_to_descriptor_2(d->descriptor, txt,
                                                         length);}



/*
 * Deal with sockets that haven't logged in yet.
 */
                            void nanny(DESCRIPTOR_DATA * d, char *argument)
                            {
                            char buf[MAX_STRING_LENGTH];
                            char kav[MAX_STRING_LENGTH];
                            CHAR_DATA * ch;
                            OBJ_DATA * obj;
                            char *pwdnew;
                            char *p;
                            char *strtime;
                            int char_age = 17;
                            bool fOld;
                            int i;
                            if (d->connected != CON_NOTE_TEXT)
                            while (isspace(*argument))
                            argument++;
                            ch = d->character; switch (d->connected)
                            {

default:
bug("Nanny: bad d->connected %d.", d->connected); close_socket(d); return; case CON_NOT_PLAYING:
break; case CON_GET_NAME:
                            if (argument[0] == '\0')
                            {
                            close_socket(d); return;}

                            if (d->lookup_status != STATUS_DONE)
                            {
                            write_to_buffer(d,
                                            "Making a dns lookup, please have patience.\n\rWhat be thy name? ",
                                            0); return;}

                            if (check_banned(d))
                            {
                            write_to_buffer(d,
                                            " Your site has been banned from this mud\n\r",
                                            0); close_socket(d); return;}

                            argument[0] = UPPER(argument[0]);
                            if (!check_parse_name(argument))
                            {
                            write_to_buffer(d,
                                            " Illegal name, try another.\n\r Enter thy name brave traveler: ",
                                            0); return;}

                            xprintf(kav, "%s trying to connect.", argument);
                            log_string(LOG_CONNECT, kav);
                            wiznet(kav, ch, NULL, WIZ_LINKS, 0, 0);
                            fOld = load_char_short(d, argument);
                            ch = d->character;
                            char_age = years_old(ch);
                            if (IS_SET(ch->act, PLR_DENY))
                            {
                            if (ch->pcdata->denied > current_time)
                            {
                            xprintf(kav, "Denying access to %s@%s.", argument,
                                    ch->lasthost);
                            wiznet(kav, ch, NULL, WIZ_SECURE, WIZ_PENALTIES,
                                   get_trust(ch));
                            log_string(LOG_SECURITY, kav);
                            write_to_buffer(d, " You are denied access.\n\r",
                                            0); close_socket(d); return;}
                            }
                            else
                            if (IS_EXTRA(ch, EXTRA_BORN) && char_age < 15)
                            {
                            char agebuf[MAX_INPUT_LENGTH];
                            if (char_age == 14)
                            xprintf_2(agebuf,
                                      "You cannot play for another year.\n\r");
                            else
                            xprintf(agebuf,
                                    "You cannot play for another %d years.\n\r",
                                    (15 - years_old(ch)));
                            write_to_buffer(d, agebuf, 0); close_socket(d);
                            return;}

                            if (IS_SET(ch->act2, PLR_WIPED))
                            {
                            close_socket(d); return;}

                            if (check_reconnect(d, argument, FALSE))
                            {
                            fOld = TRUE;}
                            else
                            {
                            /*
                             * Check max number of players - KaVir 
                             */

                            DESCRIPTOR_DATA * dcheck;
                            DESCRIPTOR_DATA * dcheck_next;
                            int countdesc = 0;
                            int max_players = 150;
                            for (dcheck = descriptor_list; dcheck != NULL;
                                 dcheck = dcheck_next)
                            {
                            dcheck_next = dcheck->next; countdesc++;}

                            if (countdesc > max_players && !IS_IMMORTAL(ch))
                            {
                            write_to_buffer(d,
                                            " Too many players connected, please try again in a couple of minutes.\n\r",
                                            0); close_socket(d); return;}

                            if (wizlock && !IS_IMMORTAL(ch))
                            {
                            write_to_buffer(d, " Currenly wizlocked.\n\r", 0);
                            close_socket(d); return;}
                            }

                            if (fOld)
                            {
                            /*
                             * Old player 
                             */
                            write_to_buffer(d, " Please enter password: ", 0);
                            write_to_buffer(d, echo_off_str, 0);
                            d->connected = CON_GET_OLD_PASSWORD; return;}
                            else
                            {
                            /*
                             * New player 
                             */

                            if (check_newbiebanned(d))
                            {
                            write_to_buffer(d,
                                            "Your site is not allowed to create new chars on this mud\n\r",
                                            0); close_socket(d); return;}
                            xprintf(buf,
                                    "Reminder: If you have chosen a name which isn't suitable, you'll get deleted.\n\r");
                            write_to_buffer(d, buf, 0);
                            xprintf(buf,
                                    "Your sure that you want %s engraved on your tombstone (Y/N)? ",
                                    argument); write_to_buffer(d, buf, 0);
                            d->connected = CON_CONFIRM_NEW_NAME; return;}
break; case CON_GET_OLD_PASSWORD:
#if defined(unix)
                            write_to_buffer(d, "\n\r", 2);
#endif
                            if (ch == NULL || (!IS_EXTRA(ch, EXTRA_NEWPASS) &&
                                               strcmp(argument,
                                                      ch->pcdata->pwd)
                                               &&
                                               strcmp(crypt
                                                      (argument,
                                                       ch->pcdata->pwd),
                                                      ch->pcdata->pwd)))
                            {
                            write_to_buffer(d, " Wrong password.\n\r", 0);
                            close_socket(d); return;}

                            else
                            if (ch == NULL || (IS_EXTRA(ch, EXTRA_NEWPASS) &&
                                               strcmp(crypt
                                                      (argument,
                                                       ch->pcdata->pwd),
                                                      ch->pcdata->pwd)))
                            {
                            write_to_buffer(d, " Wrong password.\n\r", 0);
                            close_socket(d); return;}

                            write_to_buffer(d, echo_on_str, 0);
                            if (check_reconnect(d, GET_PC_NAME(ch), TRUE))
                            return;
                            if (check_playing(d, GET_PC_NAME(ch)))
                            return; if (ch->level > 1)
                            {
                            xprintf(kav, ch->pcdata->switchname);
                            free_char(d->character);
                            d->character = NULL;
                            fOld = load_char_obj(d, kav);
                            ch = d->character; if (IS_SET(ch->act, PLR_DENY))
                            {
                            if (ch->pcdata->denied < current_time)
                            {
                            REMOVE_BIT(ch->act, PLR_DENY);
                            xprintf(kav, "Removing deny flag on %s.",
                                    ch->name);
                            wiznet(kav, ch, NULL, WIZ_PENALTIES, WIZ_SECURE,
                                   get_trust(ch));
                            log_string(LOG_SECURITY, kav);}
                            }
                            }

                            if (!IS_EXTRA(ch, EXTRA_NEWPASS)
                                && strlen(argument) > 1)
                            {
                            xprintf(kav, "%s %s", argument, argument);
                            do_password(ch, kav);}

                            if (ch->lasthost !=
                                NULL) free_string(ch->lasthost);
                            if (ch->desc != NULL && ch->desc->host != NULL)
                            {
                            ch->lasthost = str_dup(ch->desc->host);}
                            else
                            {
                            ch->lasthost = str_dup("(unknown)");}
                            strtime = ctime(&current_time);
                            strtime[strlen(strtime) - 1] = '\0';
                            free_string(ch->lasttime);
                            ch->lasttime = str_dup(strtime);
                            xprintf(kav, "%s@%s has connected.", ch->name,
                                    ch->lasthost);
                            wiznet(kav, ch, NULL, WIZ_SITES, 0,
                                   get_trust(ch));
                            log_string(LOG_CONNECT, kav);
                            /*
                             * In case we have level 4+ players from another merc mud, or 
                             * * players who have somehow got file access and changed their pfiles.
                             */
                            if (ch->level < 4 && ch->trust < 4)
                            ch->trust = 0;
                            if (ch->level > 3 && ch->trust == 0)
                            ch->level = 3;
                            else
                            {

                            if (ch->level > MAX_LEVEL)
                            ch->level = MAX_LEVEL;
                            if (ch->trust > MAX_LEVEL) ch->trust = MAX_LEVEL;}

                            ch->next = char_list;
                            char_list = ch;
                            d->connected = CON_PLAYING; if (IS_IMMORTAL(ch))
                            {
                            write_to_buffer(d,
                                            "#cWould you like to login #W(#BW#W)#cizinvis, #W(#BI#W)#cncognito, or #W(#BN#W)#cormal?#n",
                                            0); d->connected = CON_WIZ;}
                            else
                            {
                            do_help(ch, "motd");
                            if (!IS_IMMORTAL(ch))
                            d->connected = CON_CONTLOGIN;}
break; case CON_WIZ:
                            write_to_buffer(d, "\n\r", 2); switch (*argument)
                            {
case 'w':
case 'W':
(ch->invis_level = ch->level); do_help(ch, "motd"); d->connected = CON_CONTLOGIN; break; case 'i':
case 'I':
(ch->incog_level = ch->level); do_help(ch, "motd"); d->connected = CON_CONTLOGIN; break; case 'n':
case 'N':
(ch->incog_level = 0); (ch->invis_level = 0); do_help(ch, "motd"); d->connected = CON_CONTLOGIN; break; default:
                            write_to_buffer(d,
                                            "#WI#wnvalid #Ychoice#w, entering in #cnormal#w mode.#n\n\r\n\r",
                                            0);
                            do_help(ch, "motd");
                            d->connected = CON_CONTLOGIN; break;}
break; case CON_CONTLOGIN:
    /*
     * simply saving it in the pfile would be smarter - Jobo 
     */
                            d->connected = CON_PLAYING;
                            if (IS_CLASS(ch, CLASS_WEREWOLF))
                            {
                            ch->gnosis[GCURRENT] = ch->gnosis[GMAXIMUM] - 5;
                            if (ch->gnosis[GCURRENT] <
                                0) ch->gnosis[GCURRENT] = 0;}

                            if (ch->mounted == IS_RIDING) do_dismount(ch, ""); send_to_char("\n\r", ch);    // blank line to char
                            if (!IS_SET(ch->extra, EXTRA_TRUSTED))
                            SET_BIT(ch->extra, EXTRA_TRUSTED);
                            if (ch->level == 0)
                            {
                            ch->pcdata->awins = 0;  /* arena wins           */
                            ch->pcdata->alosses = 0;    /* arena losses         */
                            ch->pcdata->board = &boards[0];
                            ch->challenger = NULL;
                            ch->level = 1;
                            ch->exp = 0;
                            ch->hit = ch->max_hit;
                            ch->mana = ch->max_mana;
                            ch->move = ch->max_move;
                            ch->special = 0;
                            set_switchname(ch, ch->name);
                            char_to_room(ch,
                                         get_room_index(ROOM_VNUM_SCHOOL));
                            do_look(ch, "auto");}
                            else
                            if (!IS_NPC(ch) && ch->pcdata->obj_vnum != 0)
                            {
                            if (ch->in_room != NULL) char_to_room(ch,
                                                                  ch->
                                                                  in_room);
                            else
                            char_to_room(ch,
                                         get_room_index(ROOM_VNUM_SCHOOL));
                            bind_char(ch); break;}
                            else
                            if (ch->in_room != NULL)
                            {
                            char_to_room(ch, ch->in_room);
                            do_look(ch, "auto");}
                            else
                            {
                            char_to_room(ch,
                                         get_room_index(ROOM_VNUM_TEMPLE));
                            do_look(ch, "auto");}
                            ch->pcdata->time_tick = 0;  // resets on a _true_ login
                            ch->pcdata->safe_counter = 5;   // a few good ticks.
                            if (ch->level < 7) ch->pcdata->logout_counter = 15; // Can't logout until this expires
                            do_board(ch, ""); players_logged++; if (IS_IMMORTAL(ch));   /* no info */
                            else
                            if (IS_SET(ch->pcdata->jflags, JFLAG_SETLOGIN))
                            login_message(ch);
                            else
                            {

                            if (!ragnarok)
                            {
                            if (ch->class == CLASS_NINJA) xprintf(kav,
                                                                  "#R%s #0steps out of the shadows.#n",
                                                                  ch->name);
                            else
                            if (ch->class == CLASS_MAGE) xprintf(kav,
                                                                 "#R%s #7gates into the realm.#n",
                                                                 ch->name);
                            else
                            if (ch->class == CLASS_GHOUL) xprintf(kav,
                                                                  "#c%s #Gstaggers in, hungry for human flesh.#n",
                                                                  ch->name);
                            else
                            if (ch->class == CLASS_SKYBLADE) xprintf(kav,
                                                                     "#L%s #Cleaps down from the #Wclouds#C.#n",
                                                                     ch->
                                                                     name);
                            else
                            if (ch->class == CLASS_DEMON) xprintf(kav,
                                                                  "#r%s #Rreturns, ready to capture more #ysouls!#n",
                                                                  ch->name);
                            else
                            if (ch->class == CLASS_LICH) xprintf(kav,
                                                                 "#0%s #Pappears in an explosion of #Cm#Pa#pg#Pi#Cc!#n",
                                                                 ch->name);
                            else
                            if (ch->class == CLASS_VAMPIRE) xprintf(kav,
                                                                    "#g%s #Gstalks in, eyes #Rbu#yrni#Rng #Gwith #rb#Rloodlus#rt!#n",
                                                                    ch->name);
                            else
                            if (ch->class == CLASS_SAMURAI) xprintf(kav,
                                                                    "#B%s #cbrandishes their swords, awaiting another battle!#n",
                                                                    ch->name);
                            else
                            if (ch->class == CLASS_ANGEL) xprintf(kav,
                                                                  "#W%s #Yfloats down from #CHeaven.#n",
                                                                  ch->name);
                            else
                            if (ch->class == CLASS_SHAPESHIFTER) xprintf(kav,
                                                                         "#l%s #oshifts into being from nowhere.#n",
                                                                         ch->
                                                                         name);
                            else
                            if (ch->class == CLASS_MONK) xprintf(kav,
                                                                 "#r%s #breturns to complete their #wmartial arts #bstudies.#n",
                                                                 ch->name);
                            else
                            if (ch->class == CLASS_DROID) xprintf(kav,
                                                                  "#0%s #woils up their joints and returns for battle.#n",
                                                                  ch->name);
                            else
                            if (ch->class == CLASS_TANARRI) xprintf(kav,
                                                                    "#Y%s #RRises from hell looking for fresh meat.#n",
                                                                    ch->name);
                            else
                            if (ch->class == CLASS_CYBORG) xprintf(kav,
                                                                   "#0%s #wis done charging their batteries and returns for battle.#n",
                                                                   ch->name);
                            else
                            if (ch->class == CLASS_THIEF) xprintf(kav,
                                                                  "#0%s #wsteps from the shadows picking their teeth with a dagger.#n",
                                                                  ch->name);
                            else
                            {
                            switch (number_range(0, 5))
                            {
default:
xprintf(kav, "#w%s becomes part of the collective", ch->name); break; case 0:
xprintf(kav, "#w%s wanders in looking for the restroom.", ch->name); break; case 1:
xprintf(kav, "#wYou hear a loud #Y-=THUD=- #was %s is dropped from the #Csky.#n", ch->name); break; case 2:
xprintf(kav, "#wYou notice %s picking their butt as they enter.", ch->name); break; case 3:
xprintf(kav, "#w%s trips over the '#RWelcome#w' sign, while entering.", ch->name); break; case 4:
xprintf(kav, "#wWhile entering, %s gets wacked over the head with a blackjack.", ch->name); break; case 5:
                            xprintf(kav,
                                    "#0%s joins the collective and is assimilated'.#n",
                                    ch->name); break;}
                            }
                            enter_info(kav);}
                            else
                            {
                            xprintf(kav,
                                    "#G%s #wenters #R%s #Y(#0Ragnarok#Y).#n",
                                    ch->name, MUDNAME); enter_info(kav);}
                            }
                            if (ch->pcdata->bounty > 0 && !IS_IMMORTAL(ch))
                            {
                            xprintf(kav,
                                    "#W%s #0has a bounty of #Y%d #0bones on their #RHead#0.#n",
                                    ch->name, ch->pcdata->bounty);
                            enter_info(kav);}
                            ch->fight_timer = 0; if (ch->pcdata->quest > 0)
                            {
                            xprintf(kav,
                                    "#RYour #W%d #RQP has been converted to #W%d #yBONES#n\n\r",
                                    ch->pcdata->quest, ch->pcdata->quest * 5);
                            stc(kav, ch); ch->bones += ch->pcdata->quest * 5;
                            ch->pcdata->quest = 0;}


                            update_revision(ch);    // changed the pfile structure ?
                            act("$n has entered the game.", ch, NULL, NULL,
                                TO_ROOM); ch->pcdata->buffer = new_buf();
                            if (IS_HEAD(ch, LOST_HEAD)
                                || IS_SET(ch->extra, EXTRA_OSWITCH))
                            {
                            REMOVE_BIT(ch->loc_hp[0], LOST_HEAD);
                            REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
                            REMOVE_BIT(ch->extra, EXTRA_OSWITCH);
                            ch->morph = str_dup("");
                            ch->pcdata->chobj = NULL;
                            ch->pcdata->obj_vnum = 0;
                            char_from_room(ch);
                            char_to_room(ch,
                                         get_room_index(ROOM_VNUM_ALTAR));}

                            if (in_arena(ch) || in_fortress(ch))
                            {
                            char_from_room(ch);
                            char_to_room(ch,
                                         get_room_index(ROOM_VNUM_ALTAR));}


                            clear_buf(ch->pcdata->buffer);
                            i = num_changes(); if (i > 0)
                            {
                            xprintf(kav,
                                    "#R+++ #WThere were #R#f%d #n#Wchanges made to the mud today type #GCHANGES#W to view #R+++#n\n\r",
                                    i); stc(kav, ch);}
                            if (!IS_NPC(ch))
                            {
                            for (obj = ch->carrying; obj != NULL;
                                 obj = obj->next_content)
                            {
                            int value = 0; if (obj->item_type == ITEM_WEAPON)
                            {
                            value = obj->value[0]; if (value >= 1000)
                            {
                            int temp = value / 1000;
                            temp = temp * 1000; value -= temp;}
                            if (value == 81)
                            {
                            do_remove(ch, obj->name); extract_obj(obj);}
                            }
                            }
                            }

                            room_text(ch, ">ENTER<");
                            for (i = 0; i < MAX_DISCIPLINES; i++)
                            {
                            if (ch->power[i] > 10
                                && !IS_SET(ch->extra, EXTRA_FLASH)
                                && !IS_SET(ch->extra,
                                           EXTRA_BAAL)) ch->power[i] = 10;}

		    if( !IS_CLASS(ch, CLASS_DROW) && !IS_IMMORTAL(ch) && ch->level > 2)
		        {
		            ch->power[DISC_DROW_ARAC] = -2;
		            ch->power[DISC_DROW_JELL] = -2;
		            ch->power[DISC_DROW_RODE] = -2;
		            ch->power[DISC_DROW_BAT]  = -2;
		            ch->power[DISC_DROW_CAME] = -2;
		            ch->power[DISC_DROW_RAVE] = -2;
		            ch->power[DISC_DROW_PIRA] = -2;
		            ch->power[DISC_DROW_LOBS] = -2;
		            ch->power[DISC_DROW_HOG]  = -2;
		            ch->power[DISC_DROW_LION] = -2;
		       }


                            if (!IS_CLASS(ch, CLASS_DEMON) && !IS_IMMORTAL(ch)
                                && ch->level > 2)
                            {
                            ch->power[DISC_DAEM_ATTA] = -2;
                            ch->power[DISC_DAEM_IMMU] = -2;
                            ch->power[DISC_DAEM_TEMP] = -2;
                            ch->power[DISC_DAEM_CORR] = -2;
                            ch->power[DISC_DAEM_HELL] = -2;
                            ch->power[DISC_DAEM_GELU] = -2;
                            ch->power[DISC_DAEM_MORP] = -2;
                            ch->power[DISC_DAEM_NETH] = -2;
                            ch->power[DISC_DAEM_DISC] = -2;}
                            if (!IS_CLASS(ch, CLASS_WEREWOLF)
                                && !IS_IMMORTAL(ch) && ch->level > 2)
                            {
                            ch->power[DISC_WERE_BEAR] = -2;
                            ch->power[DISC_WERE_LYNX] = -2;
                            ch->power[DISC_WERE_BOAR] = -2;
                            ch->power[DISC_WERE_OWL] = -2;
                            ch->power[DISC_WERE_SPID] = -2;
                            ch->power[DISC_WERE_WOLF] = -2;
                            ch->power[DISC_WERE_HAWK] = -2;
                            ch->power[DISC_WERE_MANT] = -2;
                            ch->power[DISC_WERE_RAPT] = -2;
                            ch->power[DISC_WERE_LUNA] = -2;
                            ch->power[DISC_WERE_PAIN] = -2;
                            ch->power[DISC_WERE_CONG] = -2;}
                            if (!IS_CLASS(ch, CLASS_VAMPIRE)
                                && !IS_IMMORTAL(ch) && ch->level > 2)
                            {
                            ch->power[DISC_VAMP_FORT] = -2;
                            ch->power[DISC_VAMP_CELE] = -2;
                            ch->power[DISC_VAMP_OBTE] = -2;
                            ch->power[DISC_VAMP_PRES] = -2;
                            ch->power[DISC_VAMP_QUIE] = -2;
                            ch->power[DISC_VAMP_THAU] = -2;
                            ch->power[DISC_VAMP_AUSP] = -2;
                            ch->power[DISC_VAMP_DOMI] = -2;
                            ch->power[DISC_VAMP_OBFU] = -2;
                            ch->power[DISC_VAMP_POTE] = -2;
                            ch->power[DISC_VAMP_PROT] = -2;
                            ch->power[DISC_VAMP_SERP] = -2;
                            ch->power[DISC_VAMP_VICI] = -2;
                            ch->power[DISC_VAMP_DAIM] = -2;
                            ch->power[DISC_VAMP_ANIM] = -2;
                            ch->power[DISC_VAMP_CHIM] = -2;
                            ch->power[DISC_VAMP_MELP] = -2;
                            ch->power[DISC_VAMP_NECR] = -2;
                            ch->power[DISC_VAMP_THAN] = -2;
                            ch->power[DISC_VAMP_OBEA] = -2;}
                            if (!IS_CLASS(ch, CLASS_FAE) && ch->level > 2)
                            {
                            ch->power[DISC_FAE_NATURE] = -2;
                            ch->power[DISC_FAE_ARCANE] = -2;}
                            ch->embraced = NULL;
                            ch->embracing = NULL; if (multicheck(ch))
                            {
                            send_to_char
                            ("\n\r#R====#0[#Y**#0]#R====  #GWARNING  #R====#0[#Y**#0]#R====#n\n\r",
                             ch);
                            send_to_char
                            ("You have connected from the same\n\r", ch);
                            send_to_char
                            ("IP as someone who is already on the\n\r", ch);
                            send_to_char
                            ("mud. Multiplaying is illegal, and\n\r", ch);
                            send_to_char
                            ("your actions will be monitored.\n\r", ch);
                            send_to_char
                            ("#R====#0[#Y**#0]#R====  #GWARNING  #R====#0[#Y**#0]#R====#n\n\r",
                             ch);}
break; case CON_CONFIRM_NEW_NAME:
                            switch (*argument)
                            {
case 'y':
case 'Y':
xprintf(kav, " New character.\n\r Give me a password for %s: %s", ch->name, echo_off_str); write_to_buffer(d, kav, 0); d->connected = CON_GET_NEW_PASSWORD; break; case 'n':
case 'N':
write_to_buffer(d, " Ok, what IS it, then? ", 0); free_char(d->character); d->character = NULL; d->connected = CON_GET_NAME; break; default:
                            write_to_buffer(d, " Please type Yes or No? ", 0);
                            break;}
break; case CON_GET_NEW_PASSWORD:
#if defined(unix)
                            write_to_buffer(d, "\n\r", 2);
#endif
                            if (strlen(argument) < 5)
                            {
                            write_to_buffer(d,
                                            " Password must be at least five characters long.\n\r Password: ",
                                            0); return;}

                            pwdnew = crypt(argument, ch->name);
                            for (p = pwdnew; *p != '\0'; p++)
                            {
                            if (*p == '~')
                            {
                            write_to_buffer(d,
                                            "New password not acceptable, try again.\n\rPassword: ",
                                            0); return;}
                            }

free_string(ch->pcdata->pwd); ch->pcdata->pwd = str_dup(pwdnew); write_to_buffer(d, " Please retype password: ", 0); d->connected = CON_CONFIRM_NEW_PASSWORD; break; case CON_CONFIRM_NEW_PASSWORD:
#if defined(unix)
                            write_to_buffer(d, "\n\r", 2);
#endif
                            if (strcmp
                                (crypt(argument, ch->pcdata->pwd),
                                 ch->pcdata->pwd))
                            {
                            write_to_buffer(d,
                                            " Passwords don't match.\n\r Retype password: ",
                                            0);
                            d->connected = CON_GET_NEW_PASSWORD; return;}

write_to_buffer(d, echo_on_str, 0); write_to_buffer(d, " What is your sex (M/F)? ", 0); d->connected = CON_GET_NEW_SEX; break; case CON_GET_NEW_SEX:
                            switch (argument[0])
                            {
case 'm':
case 'M':
ch->sex = SEX_MALE; break; case 'f':
case 'F':
ch->sex = SEX_FEMALE; break; default:
                            write_to_buffer(d,
                                            " That's not a sex.\n\r What IS your sex? ",
                                            0); return;}
write_to_buffer(d, echo_on_str, 0); write_to_buffer(d, " Do you wish to use ANSI color (y/n)? ", 0); d->connected = CON_GET_NEW_ANSI; break; case CON_GET_NEW_ANSI:
                            switch (argument[0])
                            {
case 'y':
case 'Y':
SET_BIT(ch->act, PLR_ANSI); break; case 'n':
case 'N':
break; default:
                            write_to_buffer(d,
                                            " Do you wish to use ANSI (y/n)? ",
                                            0); return;}

ch->pcdata->perm_str = number_range(10, 16); ch->pcdata->perm_int = number_range(10, 16); ch->pcdata->perm_wis = number_range(10, 16); ch->pcdata->perm_dex = number_range(10, 16); ch->pcdata->perm_con = number_range(10, 16); ch->class = 0; set_learnable_disciplines(ch); xprintf(kav, "%s@%s new player.", ch->name, d->host); wiznet(kav, ch, NULL, WIZ_NEWBIE, 0, 0); xlogf(kav); write_to_buffer(d, "\n\r", 2); do_help(ch, "motd"); d->connected = CON_READ_MOTD; break; case CON_READ_MOTD:
                            write_to_buffer(d,
                                            "\n\r#0**#WWelcome to #RMindcloud Mud #0**\n\r\n\r#0**#WWhere #0Death#W is a part of #CLife#0**#n\n\r",
                                            0); ch->next = char_list;
                            char_list = ch; d->connected = CON_PLAYING;
                            if (IS_CLASS(ch, CLASS_WEREWOLF))
                            {
                            ch->gnosis[GCURRENT] = ch->gnosis[GMAXIMUM] - 5;
                            if (ch->gnosis[GCURRENT] <
                                0) ch->gnosis[GCURRENT] = 0;}

                            if (ch->mounted == IS_RIDING) do_dismount(ch, "");
                            if (!IS_SET(ch->extra, EXTRA_TRUSTED))
                            SET_BIT(ch->extra, EXTRA_TRUSTED);
                            if (ch->level == 0)
                            {
                            ch->pcdata->awins = 0;  /* arena wins           */
                            ch->pcdata->alosses = 0;    /* arena losses         */
                            ch->pcdata->board = &boards[0];
                            ch->challenger = NULL;
                            ch->level = 1;
                            ch->generation = 6;
                            ch->stance[19] = -1;
                            ch->stance[20] = -1;
                            ch->stance[21] = -1;
                            ch->stance[22] = -1;
                            ch->stance[23] = -1;
                            ch->exp = 0;
  			    ch->max_hit = 1000;
			    ch->max_mana = 1000;
			    ch->max_move = 1000;
                            ch->hit = ch->max_hit;
                            ch->mana = ch->max_mana;
                            ch->move = ch->max_move;
                            ch->special = 0;
                            set_switchname(ch, ch->name);
                            char_to_room(ch,
                                         get_room_index(ROOM_VNUM_SCHOOL));
                            do_look(ch, "auto"); do_newbiepack(ch, "");
                            ch->pcdata->buffer = new_buf();
                            clear_buf(ch->pcdata->buffer);}
                            else
                            if (!IS_NPC(ch) && ch->pcdata->obj_vnum != 0)
                            {
                            if (ch->in_room != NULL) char_to_room(ch,
                                                                  ch->
                                                                  in_room);
                            else
                            char_to_room(ch,
                                         get_room_index(ROOM_VNUM_SCHOOL));
                            bind_char(ch); break;}
                            else
                            if (ch->in_room != NULL)
                            {
                            char_to_room(ch, ch->in_room);
                            do_look(ch, "auto");}
                            else
                            {
                            char_to_room(ch,
                                         get_room_index(ROOM_VNUM_TEMPLE));
                            do_look(ch, "auto");}
                            do_board(ch, ""); players_logged++;
                            xprintf(kav,
                                    "#0A #RNEW #0victim#w named #G%s #0is born to #W%s.#n",
                                    ch->name, MUDNAME); enter_info(kav);
                            xprintf(kav, "A newbie named %s is born to %s.",
                                    ch->name, MUDNAME);
                            wiznet(kav, ch, NULL, WIZ_NEWBIE, 0, 0);
                            ch->fight_timer = 0; ch->pcdata->safe_counter = 0;
                            ch->pcdata->revision = CURRENT_REVISION;
                            act("$n has entered the game.", ch, NULL, NULL,
                                TO_ROOM);
                            //clear_stats(ch);
                            do_wear(ch, "all"); if (!IS_NPC(ch))
                            {
                            for (obj = ch->carrying; obj != NULL;
                                 obj = obj->next_content)
                            {
                            int value = 0;
                            if (IS_SET(obj->quest, QUEST_ARTIFACT))
                            {
                            extract_obj(obj);}
                            if (obj->item_type == ITEM_WEAPON)
                            {
                            value = obj->value[0]; if (value >= 1000)
                            {
                            int temp = value / 1000;
                            temp = temp * 1000; value -= temp;}
                            if (value == 81)
                            {
                            do_remove(ch, obj->name); extract_obj(obj);}
                            }
                            }
                            }

                            room_text(ch, ">ENTER<");
                            for (i = 0; i < MAX_DISCIPLINES; i++)
                            {
                            if (ch->power[i] > 10) ch->power[i] = 10;}

		    if( !IS_CLASS(ch, CLASS_DROW) && !IS_IMMORTAL(ch))
		        {
		            ch->power[DISC_DROW_ARAC] = -2;
		            ch->power[DISC_DROW_JELL] = -2;
		            ch->power[DISC_DROW_RODE] = -2;
		            ch->power[DISC_DROW_BAT]  = -2;
		            ch->power[DISC_DROW_CAME] = -2;
		            ch->power[DISC_DROW_RAVE] = -2;
		            ch->power[DISC_DROW_PIRA] = -2;
		            ch->power[DISC_DROW_LOBS] = -2;
		            ch->power[DISC_DROW_HOG] = -2;
		            ch->power[DISC_DROW_LION] = -2;
		       }
	
                            if (!IS_CLASS(ch, CLASS_DEMON)
                                && !IS_IMMORTAL(ch))
                            {
                            ch->power[DISC_DAEM_ATTA] = -2;
                            ch->power[DISC_DAEM_IMMU] = -2;
                            ch->power[DISC_DAEM_TEMP] = -2;
                            ch->power[DISC_DAEM_CORR] = -2;
                            ch->power[DISC_DAEM_HELL] = -2;
                            ch->power[DISC_DAEM_GELU] = -2;
                            ch->power[DISC_DAEM_MORP] = -2;
                            ch->power[DISC_DAEM_NETH] = -2;
                            ch->power[DISC_DAEM_DISC] = -2;}
                            if (!IS_CLASS(ch, CLASS_WEREWOLF)
                                && !IS_IMMORTAL(ch))
                            {
                            ch->power[DISC_WERE_BEAR] = -2;
                            ch->power[DISC_WERE_LYNX] = -2;
                            ch->power[DISC_WERE_BOAR] = -2;
                            ch->power[DISC_WERE_OWL] = -2;
                            ch->power[DISC_WERE_SPID] = -2;
                            ch->power[DISC_WERE_WOLF] = -2;
                            ch->power[DISC_WERE_HAWK] = -2;
                            ch->power[DISC_WERE_MANT] = -2;
                            ch->power[DISC_WERE_RAPT] = -2;
                            ch->power[DISC_WERE_LUNA] = -2;
                            ch->power[DISC_WERE_PAIN] = -2;
                            ch->power[DISC_WERE_CONG] = -2;}
                            if (!IS_CLASS(ch, CLASS_VAMPIRE)
                                && !IS_IMMORTAL(ch))
                            {
                            ch->power[DISC_VAMP_FORT] = -2;
                            ch->power[DISC_VAMP_CELE] = -2;
                            ch->power[DISC_VAMP_OBTE] = -2;
                            ch->power[DISC_VAMP_PRES] = -2;
                            ch->power[DISC_VAMP_QUIE] = -2;
                            ch->power[DISC_VAMP_THAU] = -2;
                            ch->power[DISC_VAMP_AUSP] = -2;
                            ch->power[DISC_VAMP_DOMI] = -2;
                            ch->power[DISC_VAMP_OBFU] = -2;
                            ch->power[DISC_VAMP_POTE] = -2;
                            ch->power[DISC_VAMP_PROT] = -2;
                            ch->power[DISC_VAMP_SERP] = -2;
                            ch->power[DISC_VAMP_VICI] = -2;
                            ch->power[DISC_VAMP_DAIM] = -2;
                            ch->power[DISC_VAMP_ANIM] = -2;
                            ch->power[DISC_VAMP_CHIM] = -2;
                            ch->power[DISC_VAMP_MELP] = -2;
                            ch->power[DISC_VAMP_NECR] = -2;
                            ch->power[DISC_VAMP_THAN] = -2;
                            ch->power[DISC_VAMP_OBEA] = -2;}
                            if (!IS_CLASS(ch, CLASS_FAE) && ch->level > 2)
                            {
                            ch->power[DISC_FAE_NATURE] = -2;
                            ch->power[DISC_FAE_ARCANE] = -2;}

                            ch->embraced = NULL;
                            ch->embracing = NULL; if (multicheck(ch))
                            {
                            send_to_char
                            ("\n\r#R====#0[#Y**#0]#R====  #GWARNING  #R====#0[#Y**#0]#R====#n\n\r",
                             ch);
                            send_to_char
                            ("You have connected from the same\n\r", ch);
                            send_to_char
                            ("IP as someone who is already on the\n\r", ch);
                            send_to_char
                            ("mud. Multiplaying is illegal, and\n\r", ch);
                            send_to_char
                            ("your actions will be monitored.\n\r", ch);
                            send_to_char
                            ("#R====#0[#Y**#0]#R====  #GWARNING  #R====#0[#Y**#0]#R====#n\n\r",
                             ch);}
                            break;
                            /*
                             * states for new note system, (c)1995-96 erwin@pip.dknet.dk 
                             */
                            /*
                             * ch MUST be PC here; have nwrite check for PC status! 
                             */
case CON_NOTE_TO:
handle_con_note_to(d, argument); break; case CON_NOTE_SUBJECT:
                            handle_con_note_subject(d, argument); break;    /* subject */
case CON_NOTE_EXPIRE:
handle_con_note_expire(d, argument); break; case CON_NOTE_TEXT:
handle_con_note_text(d, argument); break; case CON_NOTE_FINISH:
                            handle_con_note_finish(d, argument); break;}

                            return;}



/*
 * Parse a name for acceptability.
 */
                            bool check_parse_name(char *name)
                            {
                            /*
                             * Reserved words.
                             */
                            if (is_name
                                (name,
                                 "all wtf auto immortal self someone gaia none save quit why who noone"))
                            return FALSE;
                            if (is_contained("jobo", name)
                                && str_cmp(name, "jobo")) return FALSE;
                            else
                            if (is_contained("dracknuur", name)
                                && str_cmp(name, "dracknuur")) return FALSE;
                            else
                            if (is_contained("vladd", name)
                                && str_cmp(name, "vladd")) return FALSE;
                            else
                            if (is_contained("tarasque", name)
                                && str_cmp(name, "tarasque")) return FALSE;
                            else
                            if (is_contained("fuck", name)
                                || is_contained("bitch", name)
                                || is_contained("whore", name)) return FALSE;
                            /*
                             * Length restrictions.
                             */
                            if (strlen(name) < 3)
                            return FALSE; if (strlen(name) > 12) return FALSE;
                            /*
                             * Alphanumerics only.
                             * Lock out IllIll twits.
                             */
                            {
                            char *pc;
                            bool fIll;
                            fIll = TRUE; for (pc = name; *pc != '\0'; pc++)
                            {
                            if (!isalpha(*pc))
                            return FALSE;
                            if (LOWER(*pc) != 'i' && LOWER(*pc) != 'l')
                            fIll = FALSE;}

                            if (fIll) return FALSE;}

    /*
     * Prevent players from naming themselves after mobs.
     */
                            {
                            MOB_INDEX_DATA * pMobIndex;
                            int iHash;
                            for (iHash = 0; iHash < MAX_KEY_HASH; iHash++)
                            {
                            for (pMobIndex = mob_index_hash[iHash];
                                 pMobIndex != NULL;
                                 pMobIndex = pMobIndex->next)
                            {
                            if (is_name(name, pMobIndex->player_name))
                            return FALSE;}
                            }
                            }

                            return TRUE;}



/*
 * Look for link-dead player to reconnect.
 */
                            bool check_reconnect(DESCRIPTOR_DATA * d,
                                                 char *name, bool fConn)
                            {
                            CHAR_DATA * ch;
                            char buf[MSL];
                            for (ch = char_list; ch != NULL; ch = ch->next)
                            {
                            if (!IS_NPC(ch) && !IS_EXTRA(ch, EXTRA_SWITCH)
                                && (!fConn || ch->desc == NULL)
                                && !str_cmp(GET_PC_NAME(d->character),
                                            GET_PC_NAME(ch)))
                            {
                            if (fConn == FALSE)
                            {
                            free_string(d->character->pcdata->pwd);
                            d->character->pcdata->pwd =
                            str_dup(ch->pcdata->pwd);}
                            else
                            {
                            free_char(d->character);
                            d->character = ch;
                            ch->desc = d; ch->timer = 0; if (ch->tells)
                            {
                            xprintf(buf,
                                    "Reconnecting.  You have #R%d#n tells waiting.\n\r",
                                    ch->tells); send_to_char(buf, ch);
                            send_to_char("Type 'replay' to see tells.\n\r",
                                         ch);}
                            else
                            {
                            send_to_char
                            ("Reconnecting.  You have no tells waiting.\n\r",
                             ch);}
                            if (IS_NPC(ch)
                                || ch->pcdata->obj_vnum ==
                                0) act("$n has reconnected.", ch, NULL, NULL,
                                       TO_ROOM);
                            xprintf(buf, "%s@%s reconnected.", ch->name,
                                    ch->lasthost);
                            log_string(LOG_CONNECT, buf);
                            wiznet(buf, ch, NULL, WIZ_SITES, 0,
                                   get_trust(ch)); d->connected = CON_PLAYING;
                            /*
                             * Inform the character of a note in progress and the possbility of continuation! 
                             */
                            if (ch->pcdata->in_progress)
                            send_to_char
                            ("You have a note in progress. Type NWRITE to continue it.\n\r",
                             ch);}
                            return TRUE;}
                            }

                            return FALSE;}

/*
 * Kick off old connection.  KaVir.
 */
                            bool check_kickoff(DESCRIPTOR_DATA * d,
                                               char *name, bool fConn)
                            {
                            CHAR_DATA * ch;
                            char buf[MSL];
                            for (ch = char_list; ch != NULL; ch = ch->next)
                            {
                            if (!IS_NPC(ch)
                                && (!fConn || ch->desc == NULL)
                                && !str_cmp(GET_PC_NAME(d->character),
                                            GET_PC_NAME(ch)))
                            {
                            if (fConn == FALSE)
                            {
                            free_string(d->character->pcdata->pwd);
                            d->character->pcdata->pwd =
                            str_dup(ch->pcdata->pwd);}
                            else
                            {
                            free_char(d->character);
                            d->character = ch;
                            ch->desc = d;
                            ch->timer = 0;
                            send_to_char
                            ("You take over your body, which was already in use.\n\r",
                             ch);
                            act
                            ("...$n's body has been taken over by another spirit!",
                             ch, NULL, NULL, TO_ROOM);
                            xprintf(buf, "%s@%s kicking off old link.",
                                    ch->name, ch->lasthost);
                            log_string(LOG_CONNECT, buf);
                            wiznet(buf, ch, NULL, WIZ_LINKS, 0,
                                   get_trust(ch));
                            d->connected = CON_PLAYING;}
                            return TRUE;}
                            }

                            return FALSE;}



/*
 * Check if already playing - KaVir.
 * Using kickoff code from Malice, as mine is v. dodgy.
 */
                            bool check_playing(DESCRIPTOR_DATA * d,
                                               char *name)
                            {
                            DESCRIPTOR_DATA * dold;
                            for (dold = descriptor_list; dold != NULL;
                                 dold = dold->next)
                            {
                            if (dold != d
                                && dold->character != NULL
                                && dold->connected != CON_GET_NAME
                                && dold->connected != CON_GET_OLD_PASSWORD
                                && !str_cmp(name, dold->original
                                            ? GET_PC_NAME(dold->
                                                          original) :
                                            GET_PC_NAME(dold->character)))
                            {
                            char buf[MAX_STRING_LENGTH];
                            if (d->character != NULL)
                            {
                            free_char(d->character); d->character = NULL;}
                            send_to_char("This body has been taken over!\n\r", dold->character); d->connected = CON_PLAYING; d->character = dold->character; d->character->desc = d; send_to_char("You take over your body, which was already in use.\n\r", d->character); act("$n doubles over in agony and $s eyes roll up into $s head.", d->character, NULL, NULL, TO_ROOM); act("...$n's body has been taken over by another spirit!", d->character, NULL, NULL, TO_ROOM); dold->character = NULL; xprintf(buf, "Kicking off old connection %s@%s", d->character->name, d->host); log_string(LOG_CONNECT, buf); close_socket(dold);    /*Slam the old connection into the ether */
                            return TRUE;}
                            }

                            return FALSE;}



                            void stop_idling(CHAR_DATA * ch)
                            {
                            if (ch == NULL
                                || ch->desc == NULL
                                || (ch->desc->connected != CON_PLAYING
                                    && ch->desc->connected != CON_EDITING)
                                || ch->was_in_room == NULL
                                || ch->in_room !=
                                get_room_index(ROOM_VNUM_LIMBO)) return;
                            ch->timer = 0; char_from_room(ch);
                            char_to_room(ch, ch->was_in_room);
                            ch->was_in_room = NULL;
                            act("$n has returned from the void.", ch, NULL,
                                NULL, TO_ROOM); return;}


                            void stc(const char *txt, CHAR_DATA * ch)
                            {
                            if (txt != NULL && ch->desc != NULL)
                            write_to_buffer(ch->desc, txt, strlen(txt));
                            return;}

                            void sta(const char *txt, AREA_DATA * area)
                            {
                            CHAR_DATA * rch;
                            for (rch = char_list; rch != NULL;
                                 rch = rch->next)
                            {
                            if (rch->in_room->area == area) stc(txt, rch);}
                            }

                            int col_str_len(char *txt)
                            {
                            int pos, len;
                            len = 0; for (pos = 0; txt[pos] != '\0'; pos++)
                            {
                            if (txt[pos] != '#')
                            {
                            len++; continue;}

                            pos++;
                            if (txt[pos] == '\0') return len;
                            if (txt[pos] == '#' || txt[pos] == '-') len++;}

                            return len;}

                            void cent_to_char(char *txt, CHAR_DATA * ch)
                            {
                            int len, pos;
                            char buf[MAX_STRING_LENGTH];
                            len = (80 - col_str_len(txt)) / 2;
                            for (pos = 0; pos < len; pos++)
                            {
                            buf[pos] = ' ';}
                            buf[pos] = '\0';
                            send_to_char(buf, ch);
                            send_to_char(txt, ch); send_to_char("\n\r", ch);}

                            void divide_to_char(CHAR_DATA * ch)
                            {
                            send_to_char
                            ("-------------------------------------------------------------------------------\r\n",
                             ch);}

                            void divide2_to_char(CHAR_DATA * ch)
                            {
                            send_to_char
                            ("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n",
                             ch);}

                            void divide3_to_char(CHAR_DATA * ch)
                            {
                            send_to_char
                            ("#w===============================================================================#n\r\n",
                             ch);}

                            void divide4_to_char(CHAR_DATA * ch)
                            {
                            send_to_char
                            ("#w-=[#0**#w]=-=[#0**#w]=-=[#0**#w]=-=[#0**#w]=-=[#0**#w]=-=[#0**#w]=-=[#0**#w]=-=[#0**#w]=-=[#0**#w]=-=[#0**#w]=-=[#0**#w]=-\r\n",
                             ch);}

                            void divide5_to_char(CHAR_DATA * ch)
                            {
                            cent_to_char
                            ("#w-=[#b***********#w]=-------------=[#b***********#w]=-#n",
                             ch);}

                            void divide6_to_char(CHAR_DATA * ch)
                            {
                            cent_to_char
                            ("#0-    -   -  - - -#w- ---====#W*#w====--- -#0- - -  -   - -",
                             ch);}

                            void banner_to_char(char *txt, CHAR_DATA * ch)
                            {
                            char buf[MAX_STRING_LENGTH];
                            int loop, wdth, ln; ln = strlen(txt); if (ln > 16)
                            {
                            xprintf(buf,
                                    "#w-=[#0**#w]=-=[#0**#w]=-=[#0**#w]=-=[#0**#w]=-=[                     #w]=-=[#0**#w]=-=[#0**#w]=-=[#0**#w]=-=[#0**#w]=-#n");
                            wdth = (17 - ln) / 2 + 20;}
                            else
                            {
                            xprintf(buf,
                                    "#w-=[#0**#w]=-=[#0**#w]=-=[#0**#w]=-=[#0**#w]=-=[#0**#w]=-=[             ]#w=-=[#0**#w]=-=[#0**#w]=-=[#0**#w]=-=[#0**#w]=-#n");
                            wdth = (11 - ln) / 2 + 32;}
                            for (loop = 0; loop < ln; loop++)
                            buf[loop + wdth + 22] = txt[loop];
                            cent_to_char(buf, ch);}

                            void banner2_to_char(char *txt, CHAR_DATA * ch)
                            {
                            char buf[MAX_STRING_LENGTH];
                            int loop, wdth, ln; ln = strlen(txt); if (ln > 16)
                            {
                            xprintf(buf,
                                    "#0    -   -  - - -#w- ---===#W                               #w===--- -#0- - -  -   -\r\n");
                            wdth = (31 - ln) / 2 + 24;}
                            else
                            {
                            xprintf(buf,
                                    "#0     -    -   -  - - -#w- ---====#W                #w====--- -#0- - -  -   -    -\r\n");
                            wdth = (16 - ln) / 2 + 32;}
                            for (loop = 0; loop < ln;
                                 loop++) buf[loop + wdth + 6] = txt[loop];
                            send_to_char(buf, ch);}


/*
 * Write to one char.
 */
                            void send_to_char(const char *txt, CHAR_DATA * ch)
                            {
                            CHAR_DATA * wizard;
                            CHAR_DATA * familiar;
                            if (ch->desc == NULL && IS_NPC(ch)
                                && (wizard = ch->wizard) != NULL)
                            {
                            if (!IS_NPC(wizard)
                                && (familiar =
                                    wizard->pcdata->familiar) != NULL
                                && familiar == ch
                                && ch->in_room != wizard->in_room)
                            {
                            send_to_char("[ ", wizard);
                            if (txt != NULL && wizard->desc != NULL)
                            write_to_buffer(wizard->desc, txt, strlen(txt));}
                            }

                            if (txt != NULL && ch->desc != NULL)
                            write_to_buffer(ch->desc, txt, strlen(txt));
                            return;}

/* source: EOD, by John Booth */
                            void printf_to_char(CHAR_DATA * ch, char *fmt,
                                                ...)
                            {
                            char buf[MAX_STRING_LENGTH];
                            va_list args;
                            va_start(args, fmt);
                            vsprintf(buf, fmt, args);
                            va_end(args); send_to_char(buf, ch);}

/*
* The colour version of the act_new( ) function, -Lope
*/
                            void act_new(const char *format, CHAR_DATA * ch,
                                         void *arg1, void *arg2, int type)
                            {
                            static char *const he_she[] =
                            {
                            "it", "he", "she"}; static char *const him_her[] =
                            {
                            "it", "him", "her"};
                            static char *const his_her[] =
                            {
                            "its", "his", "her"};
                            CHAR_DATA * to;
                            CHAR_DATA * vch = (CHAR_DATA *) arg2;
                            OBJ_DATA * obj1 = (OBJ_DATA *) arg1;
                            OBJ_DATA * obj2 = (OBJ_DATA *) arg2;
                            BUFFER * buffer_struct;
                            const char *str;
                            char *i = NULL;
                            char *point;
                            char *pbuff;
                            char buffer[MAX_STRING_LENGTH * 2];
                            char buf[MAX_STRING_LENGTH];
                            char fname[MAX_INPUT_LENGTH];
                            /*
                             * Discard null and zero-length messages.
                             */
                            if (!format || !*format) return;
                            /*
                             * discard null rooms and chars 
                             */
                            if (!ch || !ch->in_room)
                            return;
                            to = ch->in_room->people; if (type == TO_VICT)
                            {
                            if (!vch)
                            {
                            bug("Act: null vch with TO_VICT.", 0); return;}

                            if (!vch->in_room)
                            return; to = vch->in_room->people;}

                            for (; to; to = to->next_in_room)
                            {
                            if ((!IS_NPC(to) && to->desc == NULL)
                                || (IS_NPC(to)
                                    && !HAS_TRIGGER_MOB(to, TRIG_ACT))
                                || to->position < POS_STANDING) continue;
                            if ((type == TO_CHAR) && to != ch) continue;
                            if (type == TO_VICT
                                && (to != vch || to == ch)) continue;
                            if (type == TO_ROOM && to == ch) continue;
                            if (type == TO_NOTVICT
                                && (to == ch || to == vch)) continue;
                            point = buf; str = format; while (*str != '\0')
                            {
                            if (*str != '$')
                            {
                            *point++ = *str++; continue;}

                            ++str;
                            i = " <@@@> ";
                            if (!arg2 && *str >= 'A' && *str <= 'Z')
                            {
                            bug("Act: missing arg2 for code %d.", *str);
                            i = " <@@@> ";}
                            else
                            {
                            switch (*str)
                            {
default:
                            bug("Act: bad code %d.", *str);
                            i = " <@@@> "; break;
                            /*
                             * Thx alex for 't' idea 
                             */
case 't':
i = (char *) arg1; break; case 'T':
i = (char *) arg2; break; case 'n':
i = PERS(ch, to); break; case 'N':
i = PERS(vch, to); break; case 'e':
i = he_she[URANGE(0, ch->sex, 2)]; break; case 'E':
i = he_she[URANGE(0, vch->sex, 2)]; break; case 'm':
i = him_her[URANGE(0, ch->sex, 2)]; break; case 'M':
i = him_her[URANGE(0, vch->sex, 2)]; break; case 's':
i = his_her[URANGE(0, ch->sex, 2)]; break; case 'S':
i = his_her[URANGE(0, vch->sex, 2)]; break; case 'p':
i = can_see_obj(to, obj1) ? obj1->short_descr : "something"; break; case 'P':
i = can_see_obj(to, obj2) ? obj2->short_descr : "something"; break; case 'd':
                            if (arg2 == NULL || ((char *) arg2)[0] == '\0')
                            {
                            i = "door";}
                            else
                            {
                            one_argument((char *) arg2, fname); i = fname;}
                            break;}
                            }

                            ++str; while ((*point = *i) != '\0')++ point, ++i;}

                            *point++ = '\n';
                            *point++ = '\r';
                            *point = '\0';
                            buf[0] = UPPER(buf[0]);
                            pbuff = buffer;
                            buffer_struct = new_buf(); if (to->desc != NULL)
                            {
                            add_buf(buffer_struct, buffer);
                            /*
                             * write_to_buffer( to->desc, buffer, 0 );
                             */
                            pager_to_char(buf_string(buffer_struct), to);
                            free_buf(buffer_struct);}
                            else
                            if (MOBtrigger)
                            p_act_trigger(buf, to, NULL, NULL, ch, arg1, arg2,
                                          TRIG_ACT);}
                            if (type == TO_ROOM || type == TO_NOTVICT)
                            {
                            OBJ_DATA * obj, *obj_next;
                            CHAR_DATA * tch, *tch_next;
                            point = buf; str = format; while (*str != '\0')
                            {
                            *point++ = *str++;}
                            *point = '\0';
                            for (obj = ch->in_room->contents; obj;
                                 obj = obj_next)
                            {
                            obj_next = obj->next_content;
                            if (HAS_TRIGGER_OBJ(obj, TRIG_ACT))
                            p_act_trigger(buf, NULL, obj, NULL, ch, NULL,
                                          NULL, TRIG_ACT);}

                            for (tch = ch; tch; tch = tch_next)
                            {
                            tch_next = tch->next_in_room;
                            for (obj = tch->carrying; obj; obj = obj_next)
                            {
                            obj_next = obj->next_content;
                            if (HAS_TRIGGER_OBJ(obj, TRIG_ACT))
                            p_act_trigger(buf, NULL, obj, NULL, ch, NULL,
                                          NULL, TRIG_ACT);}
                            }

                            if (HAS_TRIGGER_ROOM(ch->in_room, TRIG_ACT))
                            p_act_trigger(buf, NULL, NULL, ch->in_room, ch,
                                          NULL, NULL, TRIG_ACT);}
                            return;}

/*
 * The primary output interface for formatted output.
 */
                            void act(const char *format, CHAR_DATA * ch,
                                     void *arg1, void *arg2, int type)
                            {
                            static char *const he_she[] =
                            {
                            "it", "he", "she"}; static char *const him_her[] =
                            {
                            "it", "him", "her"};
                            static char *const his_her[] =
                            {
                            "its", "his", "her"};
                            char buf[MAX_STRING_LENGTH];
                            char fname[MAX_INPUT_LENGTH];
                            CHAR_DATA * to;
                            CHAR_DATA * to_old;
                            CHAR_DATA * vch = (CHAR_DATA *) arg2;
                            CHAR_DATA * familiar = NULL;
                            CHAR_DATA * wizard = NULL;
                            //BUFFER *buffer_struct;
                            OBJ_DATA * obj1 = (OBJ_DATA *) arg1;
                            OBJ_DATA * obj2 = (OBJ_DATA *) arg2;
                            //char      *pbuff;
                            //char      buffer[ MAX_STRING_LENGTH*2 ];
                            const char *str;
                            const char *i;
                            char *point; bool is_fam; bool is_ok;
                            /*
                             * Discard null and zero-length messages.
                             */
                            if (format == NULL || format[0] == '\0') return;
/*	Skummel crash bug, tror dette check spiser buggen, vi faar se */
                            if (ch == NULL || ch->in_room == NULL)
                            {
                            return;}

                            to = ch->in_room->people; if (type == TO_VICT)
                            {
                            if (vch == NULL || vch->in_room == NULL)
                            {
/*	    bug( "Act: null vch with TO_VICT.", 0 );*/
                            return;}
                            to = vch->in_room->people;}

                            for (; to != NULL; to = to->next_in_room)
                            {

                            is_fam = FALSE;
                            to_old = to;
                            if ((!IS_NPC(to) && to->desc == NULL)
                                || (IS_NPC(to)
                                    && !HAS_TRIGGER_MOB(to,
                                                        TRIG_ACT))) continue;
                            if (type == TO_CHAR && to != ch) continue;
                            if (type == TO_VICT
                                && (to != vch || to == ch)) continue;
                            if (type == TO_ROOM && to == ch) continue;
                            if (type == TO_NOTVICT
                                && (to == ch || to == vch)) continue;
                            if (to->desc == NULL && IS_NPC(to)
                                && (wizard = to->wizard) != NULL)
                            {
                            if (!IS_NPC(wizard)
                                && ((familiar = wizard->pcdata->familiar) !=
                                    NULL) && familiar == to)
                            {
                            if (to->in_room == ch->in_room &&
                                wizard->in_room != to->in_room)
                            {
                            to = wizard; is_fam = TRUE;}
                            }
                            }

                            if (to->desc == NULL || !IS_AWAKE(to))
                            {

                            if (is_fam) to = to_old; continue;}

                            if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
                            {
                            is_ok = FALSE;
                            if (!IS_NPC(ch) && ch->pcdata->chobj != NULL &&
                                ch->pcdata->chobj->in_room != NULL &&
                                !IS_NPC(to) && to->pcdata->chobj != NULL &&
                                to->pcdata->chobj->in_room != NULL &&
                                ch->in_room == to->in_room) is_ok = TRUE;
                            else
                            is_ok = FALSE;
                            if (!IS_NPC(ch) && ch->pcdata->chobj != NULL &&
                                ch->pcdata->chobj->in_obj != NULL &&
                                !IS_NPC(to) && to->pcdata->chobj != NULL &&
                                to->pcdata->chobj->in_obj != NULL &&
                                ch->pcdata->chobj->in_obj ==
                                to->pcdata->chobj->in_obj) is_ok = TRUE;
                            else
                            is_ok = FALSE; if (!is_ok)
                            {

                            if (is_fam) to = to_old; continue;}
                            }

                            point = buf; str = format; while (*str != '\0')
                            {
                            if (*str != '$')
                            {
                            *point++ = *str++; continue;}
                            ++str;
                            if (arg2 == NULL && *str >= 'A' && *str <= 'Z')
                            {
/*		bug( "Act: missing arg2 for code %d.", *str );*/
                            i = " <@@@> ";}
                            else
                            {
                            switch (*str)
                            {
default:
                            bug("Act: bad code %d.", *str);
                            i = " <@@@> "; break;
                            /*
                             * Thx alex for 't' idea 
                             */
#ifdef I3
case '$':
                            i = "$"; break;
#endif
case 't':
i = (char *) arg1; break; case 'T':
i = (char *) arg2; break; case 'n':
i = PERS(ch, to); break; case 'N':
i = PERS(vch, to); break; case 'e':
i = he_she[URANGE(0, ch->sex, 2)]; break; case 'E':
i = he_she[URANGE(0, vch->sex, 2)]; break; case 'm':
i = him_her[URANGE(0, ch->sex, 2)]; break; case 'M':
i = him_her[URANGE(0, vch->sex, 2)]; break; case 's':
i = his_her[URANGE(0, ch->sex, 2)]; break; case 'S':
i = his_her[URANGE(0, vch->sex, 2)]; break; case 'p':
i = can_see_obj(to, obj1) ? ((obj1->chobj != NULL && obj1->chobj == to) ? "you" : obj1->short_descr) : "something"; break; case 'P':
i = can_see_obj(to, obj2) ? ((obj2->chobj != NULL && obj2->chobj == to) ? "you" : obj2->short_descr) : "something"; break; case 'd':
                            if (arg2 == NULL || ((char *) arg2)[0] == '\0')
                            {
                            i = "door";}
                            else
                            {
                            one_argument((char *) arg2, fname); i = fname;}
                            break;}
                            }

                            ++str; while ((*point = *i) != '\0')++ point, ++i;}

                            *point++ = '\n'; *point++ = '\r';
                            //*point   = '\0';
                            buf[0] = UPPER(buf[0]);
                            //pbuff  = buffer;
                            if (is_fam)
                            {
                            if (to->in_room != ch->in_room && familiar != NULL
                                && familiar->in_room ==
                                ch->in_room) send_to_char("[ ", to);
                            else
                            {
                            to = to_old; continue;}
                            }

                            //buffer_struct = new_buf();

                            //if ( to->desc != NULL )
                            //{
                            //      add_buf(buffer_struct,buffer);
                            //      /*write_to_buffer( to->desc, buffer, 0 );*/
                            //      pager_to_char(buf_string(buffer_struct),to);
                            //      free_buf(buffer_struct);
                            //}
                            //  else
                            //      if ( MOBtrigger )
                            //          p_act_trigger( buf, to, NULL, NULL, ch, arg1, arg2, TRIG_ACT );

                            buf[0] = UPPER(buf[0]); if (to->desc != NULL)
                            {
                            if (to->desc
                                && (to->desc->connected ==
                                    CON_PLAYING)) write_to_buffer(to->desc,
                                                                  buf,
                                                                  point -
                                                                  buf);}
                            else
                            if (MOBtrigger)
                            p_act_trigger(buf, to, NULL, NULL, ch, arg1, arg2,
                                          TRIG_ACT);
                            if (type == TO_ROOM || type == TO_NOTVICT)
                            {
                            OBJ_DATA * obj, *obj_next;
                            CHAR_DATA * tch, *tch_next;
                            point = buf; str = format; while (*str != '\0')
                            {
                            *point++ = *str++;}
                            *point = '\0';
                            for (obj = ch->in_room->contents; obj;
                                 obj = obj_next)
                            {
                            obj_next = obj->next_content;
                            if (HAS_TRIGGER_OBJ(obj, TRIG_ACT))
                            p_act_trigger(buf, NULL, obj, NULL, ch, NULL,
                                          NULL, TRIG_ACT);}

                            for (tch = ch; tch; tch = tch_next)
                            {
                            tch_next = tch->next_in_room;
                            for (obj = tch->carrying; obj; obj = obj_next)
                            {
                            obj_next = obj->next_content;
                            if (HAS_TRIGGER_OBJ(obj, TRIG_ACT))
                            p_act_trigger(buf, NULL, obj, NULL, ch, NULL,
                                          NULL, TRIG_ACT);}
                            }

                            if (HAS_TRIGGER_ROOM(ch->in_room, TRIG_ACT))
                            p_act_trigger(buf, NULL, NULL, ch->in_room, ch,
                                          NULL, NULL, TRIG_ACT);}

                            //buf[0]   = UPPER(buf[0]);
                            //if (to->desc && (to->desc->connected == CON_PLAYING))
                            //write_to_buffer( to->desc, buf, point - buf );
                            //else
                            //      if ( MOBtrigger )
                            //          p_act_trigger( buf, to, NULL, NULL, ch, arg1, arg2, TRIG_ACT );
                            if (is_fam) to = to_old;}
                            return;}



                            void act2(const char *format, CHAR_DATA * ch,
                                      void *arg1, void *arg2, int type)
                            {
                            static char *const he_she[] =
                            {
                            "it", "he", "she"}; static char *const him_her[] =
                            {
                            "it", "him", "her"};
                            static char *const his_her[] =
                            {
                            "its", "his", "her"};
                            char buf[MAX_STRING_LENGTH];
                            CHAR_DATA * to;
                            CHAR_DATA * to_old;
                            CHAR_DATA * vch = (CHAR_DATA *) arg2;
                            CHAR_DATA * familiar = NULL;
                            CHAR_DATA * wizard = NULL;
                            OBJ_DATA * obj1 = (OBJ_DATA *) arg1;
                            OBJ_DATA * obj2 = (OBJ_DATA *) arg2;
                            const char *str;
                            const char *i;
                            char *point; bool is_fam; bool is_ok;
                            /*
                             * Discard null and zero-length messages.
                             */
                            if (format == NULL || format[0] == '\0')
                            return;
                            to = ch->in_room->people; if (type == TO_VICT)
                            {
                            if (vch == NULL || vch->in_room == NULL)
                            {
                            bug("Act: null vch with TO_VICT.", 0); return;}
                            to = vch->in_room->people;}

                            for (; to != NULL; to = to->next_in_room)
                            {

                            is_fam = FALSE;
                            to_old = to;
                            if ((!IS_NPC(to) && to->desc == NULL)
                                || (IS_NPC(to)
                                    && !HAS_TRIGGER_MOB(to,
                                                        TRIG_ACT))) continue;
                            if (type == TO_CHAR && to != ch) continue;
                            if (type == TO_VICT
                                && (to != vch || to == ch)) continue;
                            if (type == TO_ROOM && to == ch) continue;
                            if (type == TO_NOTVICT
                                && (to == ch || to == vch)) continue;
                            if (to->desc == NULL && IS_NPC(to)
                                && (wizard = to->wizard) != NULL)
                            {
                            if (!IS_NPC(wizard)
                                && ((familiar = wizard->pcdata->familiar) !=
                                    NULL) && familiar == to)
                            {
                            if (to->in_room == ch->in_room &&
                                wizard->in_room != to->in_room)
                            {
                            to = wizard; is_fam = TRUE;}
                            }
                            }

                            if (to->desc == NULL || !IS_AWAKE(to))
                            {

                            if (is_fam) to = to_old; continue;}

                            if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
                            {
                            is_ok = FALSE;
                            if (!IS_NPC(ch) && ch->pcdata->chobj != NULL &&
                                ch->pcdata->chobj->in_room != NULL &&
                                !IS_NPC(to) && to->pcdata->chobj != NULL &&
                                to->pcdata->chobj->in_room != NULL &&
                                ch->in_room == to->in_room) is_ok = TRUE;
                            else
                            is_ok = FALSE;
                            if (!IS_NPC(ch) && ch->pcdata->chobj != NULL &&
                                ch->pcdata->chobj->in_obj != NULL &&
                                !IS_NPC(to) && to->pcdata->chobj != NULL &&
                                to->pcdata->chobj->in_obj != NULL &&
                                ch->pcdata->chobj->in_obj ==
                                to->pcdata->chobj->in_obj) is_ok = TRUE;
                            else
                            is_ok = FALSE; if (!is_ok)
                            {

                            if (is_fam) to = to_old; continue;}
                            }

                            point = buf; str = format; while (*str != '\0')
                            {
                            if (*str != '$')
                            {
                            *point++ = *str++; continue;}
                            ++str;
                            if (arg2 == NULL && *str >= 'A' && *str <= 'Z')
                            {

/*		bug( "Act: missing arg2 for code %d.", *str );
*/
                            i = " <@@@> ";}
                            else
                            {
                            switch (*str)
                            {
default:
i = " "; break; case 'n':
                            if (ch != NULL) i = PERS(ch, to);
                            else
i = " "; break; case 'N':
                            if (vch != NULL) i = PERS(vch, to);
                            else
i = " "; break; case 'e':
                            if (ch != NULL) i = he_she[URANGE(0, ch->sex, 2)];
                            else
i = " "; break; case 'E':
                            if (vch != NULL) i =
                            he_she[URANGE(0, vch->sex, 2)];
                            else
i = " "; break; case 'm':
                            if (ch != NULL) i =
                            him_her[URANGE(0, ch->sex, 2)];
                            else
i = " "; break; case 'M':
                            if (vch != NULL) i =
                            him_her[URANGE(0, vch->sex, 2)];
                            else
i = " "; break; case 's':
                            if (ch != NULL) i =
                            his_her[URANGE(0, ch->sex, 2)];
                            else
i = " "; break; case 'S':
                            if (vch != NULL) i =
                            his_her[URANGE(0, vch->sex, 2)];
                            else
i = " "; break; case 'p':
                            if (obj1 != NULL)
                            {
                            i = can_see_obj(to, obj1)
                            ? ((obj1->chobj != NULL && obj1->chobj == to)
                               ? "you" : obj1->short_descr) : "something";}
                            else
i = " "; break; case 'P':
                            if (obj2 != NULL)
                            {
                            i = can_see_obj(to, obj2)
                            ? ((obj2->chobj != NULL && obj2->chobj == to)
                               ? "you" : obj2->short_descr) : "something";}
                            else
                            i = " "; break;}
                            }

                            ++str; while ((*point = *i) != '\0')++ point, ++i;}

                            *point++ = '\n'; *point++ = '\r'; if (is_fam)
                            {
                            if (to->in_room != ch->in_room && familiar != NULL
                                && familiar->in_room ==
                                ch->in_room) send_to_char("[ ", to);
                            else
                            {
                            to = to_old; continue;}
                            }

                            buf[0] = UPPER(buf[0]);
                            if (to->desc
                                && (to->desc->connected ==
                                    CON_PLAYING)) write_to_buffer(to->desc,
                                                                  buf,
                                                                  point -
                                                                  buf);
                            else
                            if (MOBtrigger)
                            p_act_trigger(buf, to, NULL, NULL, ch, arg1, arg2,
                                          TRIG_ACT); if (is_fam) to = to_old;}
                            return;}

                            void act_brief(const char *format, CHAR_DATA * ch,
                                           void *arg1, void *arg2, int type,
                                           int iBrief)
                            {
                            static char *const he_she[] =
                            {
                            "it", "he", "she"}; static char *const him_her[] =
                            {
                            "it", "him", "her"};
                            static char *const his_her[] =
                            {
                            "its", "his", "her"};
                            char buf[MAX_STRING_LENGTH];
                            char fname[MAX_INPUT_LENGTH];
                            CHAR_DATA * to;
                            CHAR_DATA * to_old;
                            CHAR_DATA * vch = (CHAR_DATA *) arg2;
                            CHAR_DATA * familiar = NULL;
                            CHAR_DATA * wizard = NULL;
                            OBJ_DATA * obj1 = (OBJ_DATA *) arg1;
                            OBJ_DATA * obj2 = (OBJ_DATA *) arg2;
                            const char *str;
                            const char *i;
                            char *point; bool is_fam; bool is_ok;
                            /*
                             * Discard null and zero-length messages.
                             */
                            if (format == NULL || format[0] == '\0')
                            return;
                            if (ch == NULL || ch->in_room == NULL) return;
                            to = ch->in_room->people; if (type == TO_VICT)
                            {
                            if (vch == NULL || vch->in_room == NULL) return;
                            to = vch->in_room->people;}

                            for (; to != NULL; to = to->next_in_room)
                            {

                            is_fam = FALSE;
                            to_old = to;
                            if (type == TO_CHAR && to != ch) continue;
                            if (type == TO_VICT
                                && (to != vch || to == ch)) continue;
                            if (type == TO_ROOM && to == ch) continue;
                            if (type == TO_NOTVICT
                                && (to == ch || to == vch)) continue;
                            if (to->desc == NULL && IS_NPC(to)
                                && (wizard = to->wizard) != NULL)
                            {
                            if (!IS_NPC(wizard)
                                && ((familiar = wizard->pcdata->familiar) !=
                                    NULL) && familiar == to)
                            {
                            if (to->in_room == ch->in_room &&
                                wizard->in_room != to->in_room)
                            {
                            to = wizard; is_fam = TRUE;}
                            }
                            }
                            if (to->desc == NULL || !IS_AWAKE(to))
                            {
                            if (is_fam) to = to_old; continue;}
                            if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
                            {
                            is_ok = FALSE;
                            if (!IS_NPC(ch) && ch->pcdata->chobj != NULL &&
                                ch->pcdata->chobj->in_room != NULL &&
                                !IS_NPC(to) && to->pcdata->chobj != NULL &&
                                to->pcdata->chobj->in_room != NULL &&
                                ch->in_room == to->in_room) is_ok = TRUE;
                            else
                            is_ok = FALSE;
                            if (!IS_NPC(ch) && ch->pcdata->chobj != NULL &&
                                ch->pcdata->chobj->in_obj != NULL &&
                                !IS_NPC(to) && to->pcdata->chobj != NULL &&
                                to->pcdata->chobj->in_obj != NULL &&
                                ch->pcdata->chobj->in_obj ==
                                to->pcdata->chobj->in_obj) is_ok = TRUE;
                            else
                            is_ok = FALSE; if (!is_ok)
                            {
                            if (is_fam) to = to_old; continue;}
                            }
                            /*
                             * Checking for brief
                             */
                            if (!IS_NPC(to)
                                && IS_SET(to->act, iBrief)) continue;
                            point = buf; str = format; while (*str != '\0')
                            {
                            if (*str != '$')
                            {
                            *point++ = *str++; continue;}
                            ++str;
                            if (arg2 == NULL && *str >= 'A'
                                && *str <= 'Z') i = " <@@@> ";
                            else
                            {
                            switch (*str)
                            {
default:
                            bug("Act: bad code %d.", *str);
                            i = " <@@@> "; break;
                            /*
                             * Thx alex for 't' idea 
                             */
#ifdef I3
case '$':
                            i = "$"; break;
#endif
case 't':
i = (char *) arg1; break; case 'T':
i = (char *) arg2; break; case 'n':
i = PERS(ch, to); break; case 'N':
i = PERS(vch, to); break; case 'e':
i = he_she[URANGE(0, ch->sex, 2)]; break; case 'E':
i = he_she[URANGE(0, vch->sex, 2)]; break; case 'm':
i = him_her[URANGE(0, ch->sex, 2)]; break; case 'M':
i = him_her[URANGE(0, vch->sex, 2)]; break; case 's':
i = his_her[URANGE(0, ch->sex, 2)]; break; case 'S':
i = his_her[URANGE(0, vch->sex, 2)]; break; case 'p':
i = can_see_obj(to, obj1) ? ((obj1->chobj != NULL && obj1->chobj == to) ? "you" : obj1->short_descr) : "something"; break; case 'P':
i = can_see_obj(to, obj2) ? ((obj2->chobj != NULL && obj2->chobj == to) ? "you" : obj2->short_descr) : "something"; break; case 'd':
                            if (arg2 == NULL || ((char *) arg2)[0] == '\0')
                            {
                            i = "door";}
                            else
                            {
                            one_argument((char *) arg2, fname); i = fname;}
                            break;}
                            }
                            ++str; while ((*point = *i) != '\0')++ point, ++i;}

                            *point++ = '\n'; *point++ = '\r'; if (is_fam)
                            {
                            if (to->in_room != ch->in_room && familiar != NULL
                                && familiar->in_room ==
                                ch->in_room) send_to_char("[ ", to);
                            else
                            {
                            to = to_old; continue;}
                            }

                            buf[0] = UPPER(buf[0]);
                            if (to->desc
                                && (to->desc->connected ==
                                    CON_PLAYING)) write_to_buffer(to->desc,
                                                                  buf,
                                                                  point -
                                                                  buf);
                            if (is_fam) to = to_old;}
                            return;}

                            void kavitem(const char *format, CHAR_DATA * ch,
                                         void *arg1, void *arg2, int type)
                            {
                            static char *const he_she[] =
                            {
                            "it", "he", "she"}; static char *const him_her[] =
                            {
                            "it", "him", "her"};
                            static char *const his_her[] =
                            {
                            "its", "his", "her"};
                            char buf[MAX_STRING_LENGTH];
                            char kav[MAX_INPUT_LENGTH];
                            CHAR_DATA * to;
                            CHAR_DATA * vch = (CHAR_DATA *) arg2;
                            OBJ_DATA * obj1 = (OBJ_DATA *) arg1;
                            const char *str;
                            const char *i; char *point; bool is_ok;
                            /*
                             * Discard null and zero-length messages.
                             */
                            if (format == NULL || format[0] == '\0')
                            return;
                            to = ch->in_room->people; if (type == TO_VICT)
                            {
                            if (vch == NULL)
                            {
                            bug("Act: null vch with TO_VICT.", 0); return;}
                            to = vch->in_room->people;}

                            for (; to != NULL; to = to->next_in_room)
                            {
                            if (to->desc == NULL || !IS_AWAKE(to))
                            continue;
                            if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
                            {
                            is_ok = FALSE;
                            if (!IS_NPC(ch) && ch->pcdata->chobj != NULL &&
                                ch->pcdata->chobj->in_room != NULL &&
                                !IS_NPC(to) && to->pcdata->chobj != NULL &&
                                to->pcdata->chobj->in_room != NULL &&
                                ch->in_room == to->in_room) is_ok = TRUE;
                            else
                            is_ok = FALSE;
                            if (!IS_NPC(ch) && ch->pcdata->chobj != NULL &&
                                ch->pcdata->chobj->in_obj != NULL &&
                                !IS_NPC(to) && to->pcdata->chobj != NULL &&
                                to->pcdata->chobj->in_obj != NULL &&
                                ch->pcdata->chobj->in_obj ==
                                to->pcdata->chobj->in_obj) is_ok = TRUE;
                            else
                            is_ok = FALSE; if (!is_ok) continue;}
                            if (type == TO_CHAR && to != ch)
                            continue;
                            if (type == TO_VICT && (to != vch || to == ch))
                            continue;
                            if (type == TO_ROOM && to == ch)
                            continue;
                            if (type == TO_NOTVICT && (to == ch || to == vch))
                            continue;
                            point = buf; str = format; while (*str != '\0')
                            {
                            if (*str != '$')
                            {
                            *point++ = *str++; continue;}
                            ++str;
                            if (arg2 == NULL && *str >= 'A' && *str <= 'Z')
                            i = "";
                            else
                            {
                            switch (*str)
                            {
default:
i = ""; break; case 'n':
i = PERS(ch, to); break; case 'e':
i = he_she[URANGE(0, ch->sex, 2)]; break; case 'm':
i = him_her[URANGE(0, ch->sex, 2)]; break; case 's':
i = his_her[URANGE(0, ch->sex, 2)]; break; case 'p':
i = can_see_obj(to, obj1) ? ((obj1->chobj != NULL && obj1->chobj == to) ? "you" : obj1->short_descr) : "something"; break; case 'o':
                            if (obj1 != NULL) xprintf(kav, "%s's",
                                                      obj1->short_descr);
                            i =
                            can_see_obj(to,
                                        obj1) ? ((obj1->chobj != NULL
                                                  && obj1->chobj ==
                                                  to) ? "your" : kav) :
                            "something's"; break;}
                            }

                            ++str; while ((*point = *i) != '\0')++ point, ++i;}

                            *point++ = '\n';
                            *point++ = '\r';
                            buf[0] = UPPER(buf[0]);
                            write_to_buffer(to->desc, buf, point - buf);}

                            return;}

                            void bust_a_header(DESCRIPTOR_DATA * d)
                            {
                            char class[MAX_STRING_LENGTH];
                            char class2[MAX_STRING_LENGTH];
                            char header[MAX_STRING_LENGTH];
                            char header1[MAX_STRING_LENGTH];
                            char blanklin[MAX_STRING_LENGTH];
                            CHAR_DATA * ch;
                            char cls[MAX_STRING_LENGTH];
                            xprintf(cls, " ");
                            ch = d->character;
                            if (ch == NULL) return;
                            ADD_COLOUR(ch, cls, NORMAL);
                            if (IS_CLASS(ch, CLASS_VAMPIRE)) xprintf(class,
                                                                     "Vampire");
                            else
                            if (IS_CLASS(ch, CLASS_DEMON)) xprintf(class,
                                                                   "Demon");
                            else
                            if (IS_CLASS(ch, CLASS_WEREWOLF)) xprintf(class,
                                                                      "Werewolf");
                            else
                            xprintf(class, "Classless");
                            xprintf(class2, "%s the %s", ch->name, class);
                            xprintf(blanklin, " ");
                            xprintf(header1, "%-30s Align:%-4d", class2,
                                    ch->alignment);
                            xprintf(header,
                                    "\0337\033[1;1H\033[1;44m\033[1;37m%-79s%s\0338",
                                    header1, cls); send_to_char(header, ch);
                            return;}
/*
 * Bust a prompt (player settable prompt)
 * coded by Morgenes for Aldara Mud
 */
                            void bust_a_prompt(DESCRIPTOR_DATA * d)
                            {
                            CHAR_DATA * ch;
                            CHAR_DATA * victim;
                            CHAR_DATA * tank;
                            OBJ_DATA * obj;
                            const char *str;
                            const char *i;
                            char *point;
                            char buf[MAX_STRING_LENGTH];
                            char buf2[MAX_STRING_LENGTH];
                            bool is_fighting = TRUE;
                            int members = 0;
                            if ((ch = d->character) == NULL) return;
                            if (ch->pcdata == NULL)
                            {
                            send_to_char("\n\r\n\r", ch); return;}
                            if (ch->position == POS_FIGHTING
                                && ch->cprompt[0] == '\0')
                            {
                            if (ch->prompt[0] == '\0')
                            {
                            send_to_char("\n\r\n\r", ch); return;}
                            is_fighting = FALSE;}
                            else
                            if (ch->position != POS_FIGHTING
                                && ch->prompt[0] == '\0')
                            {
                            send_to_char("\n\r\n\r", ch); return;}

                            point = buf;
                            if (ch->position == POS_FIGHTING && is_fighting)
                            str =
                            d->original ? d->original->cprompt : d->
                            character->cprompt;
                            else
                            str =
                            d->original ? d->original->prompt : d->character->
                            prompt; while (*str != '\0')
                            {
                            if (*str != '%')
                            {
                            *point++ = *str++; continue;}
                            ++str; switch (*str)
                            {
default:
i = " "; break; case 'h':
xprintf(buf2, "%d", ch->hit); COL_SCALE(buf2, ch, ch->hit, ch->max_hit); i = buf2; break; case 'H':
xprintf(buf2, "%d", ch->max_hit); ADD_COLOUR(ch, buf2, L_CYAN); i = buf2; break; case 'm':
xprintf(buf2, "%d", ch->mana); COL_SCALE(buf2, ch, ch->mana, ch->max_mana); i = buf2; break; case 'M':
xprintf(buf2, "%d", ch->max_mana); ADD_COLOUR(ch, buf2, L_CYAN); i = buf2; break; case 'v':
xprintf(buf2, "%d", ch->move); COL_SCALE(buf2, ch, ch->move, ch->max_move); i = buf2; break; case 'V':
xprintf(buf2, "%d", ch->max_move); ADD_COLOUR(ch, buf2, L_CYAN); i = buf2; break; case 'x':
xprintf(buf2, "%2.0f", ch->exp); //COL_SCALE(buf2, ch, ch->exp, 1000); i = buf2; break; case 'X':
xprintf(buf2, "\n\r"); i = buf2; break; case 'q':
xprintf(buf2, "%d", ch->bones); ADD_COLOUR(ch, buf2, L_CYAN); i = buf2; break; case 'Q':
                            if (!IS_NPC(ch))
                            xprintf(buf2, "%d",
                                    ch->pcdata->stats[SHADOW_POWER]);
                            else
xprintf(buf2, " "); i = buf2; break; case 'f':
                            if ((victim = ch->fighting) == NULL)
                            {
                            xprintf(buf2, "N/A");
                            ADD_COLOUR(ch, buf2, L_CYAN);}
                            else
                            {
                            if ((victim->hit * 100 / victim->max_hit) < 25)
                            {
                            xprintf(buf2, "Awful");
                            ADD_COLOUR(ch, buf2, L_RED);}
                            else
                            if ((victim->hit * 100 / victim->max_hit) < 35)
                            {
                            xprintf(buf2, "Hurting");
                            ADD_COLOUR(ch, buf2, D_RED);}
                            else
                            if ((victim->hit * 100 / victim->max_hit) < 50)
                            {
                            xprintf(buf2, "Poor");
                            ADD_COLOUR(ch, buf2, D_RED);}
                            else
                            if ((victim->hit * 100 / victim->max_hit) < 75)
                            {
                            xprintf(buf2, "Wounded");
                            ADD_COLOUR(ch, buf2, BROWN);}
                            else
                            if ((victim->hit * 100 / victim->max_hit) < 85)
                            {
                            xprintf(buf2, "Bruised");
                            ADD_COLOUR(ch, buf2, YELLOW);}
                            else
                            if ((victim->hit * 100 / victim->max_hit) < 100)
                            {
                            xprintf(buf2, "Good");
                            ADD_COLOUR(ch, buf2, D_GREEN);}
                            else
                            if ((victim->hit * 100 / victim->max_hit) >= 100)
                            {
                            xprintf(buf2, "Perfect");
                            ADD_COLOUR(ch, buf2, L_GREEN);}
                            }
i = buf2; break; case 'F':
                            if ((victim = ch->fighting) == NULL)
                            {
                            xprintf(buf2, "N/A");
                            ADD_COLOUR(ch, buf2, L_CYAN);}
                            else
                            if ((tank = victim->fighting) == NULL)
                            {
                            xprintf(buf2, "N/A");
                            ADD_COLOUR(ch, buf2, L_CYAN);}
                            else
                            {
                            if ((tank->hit * 100 / tank->max_hit) < 25)
                            {
                            xprintf(buf2, "Awful");
                            ADD_COLOUR(ch, buf2, L_RED);}
                            else
                            if ((tank->hit * 100 / tank->max_hit) < 50)
                            {
                            xprintf(buf2, "Poor");
                            ADD_COLOUR(ch, buf2, L_BLUE);}
                            else
                            if ((tank->hit * 100 / tank->max_hit) < 75)
                            {
                            xprintf(buf2, "Fair");
                            ADD_COLOUR(ch, buf2, L_GREEN);}
                            else
                            if ((tank->hit * 100 / tank->max_hit) < 100)
                            {
                            xprintf(buf2, "Good");
                            ADD_COLOUR(ch, buf2, YELLOW);}
                            else
                            if ((tank->hit * 100 / tank->max_hit) >= 100)
                            {
                            xprintf(buf2, "Perfect");
                            ADD_COLOUR(ch, buf2, L_CYAN);}
                            }
i = buf2; break; case 'n':
                            if ((victim = ch->fighting) == NULL)
                            xprintf(buf2, "N/A");
                            else
                            {
                            if (IS_AFFECTED(victim, AFF_POLYMORPH))
                            strcpy(buf2, victim->morph);
                            else
                            if (IS_NPC(victim))
                            strcpy(buf2, victim->short_descr);
                            else
                            strcpy(buf2, victim->name);
                            buf2[0] = UPPER(buf2[0]);}
i = buf2; break; case 'N':
                            if ((victim = ch->fighting) == NULL)
                            xprintf(buf2, "N/A");
                            else
                            if ((tank = victim->fighting) == NULL)
                            xprintf(buf2, "N/A");
                            else
                            {
                            if (ch == tank) xprintf(buf2, "You");
                            else
                            if (IS_AFFECTED(tank, AFF_POLYMORPH))
                            strcpy(buf2, tank->morph);
                            else
                            if (IS_NPC(victim))
                            strcpy(buf2, tank->short_descr);
                            else
                            strcpy(buf2, tank->name);
                            buf2[0] = UPPER(buf2[0]);}
i = buf2; break; case 't':
xprintf(buf2, "%d", ch->fight_timer); ADD_COLOUR(ch, buf2, L_CYAN); i = buf2; break; case 'T':
                            switch (ch->stance[0])
                            {
case 1:
xprintf(buf2, "Viper"); break; case 2:
xprintf(buf2, "Crane"); break; case 3:
xprintf(buf2, "Crab"); break; case 4:
xprintf(buf2, "Mongoose"); break; case 5:
xprintf(buf2, "Bull"); break; case 6:
xprintf(buf2, "Mantis"); break; case 7:
xprintf(buf2, "Dragon"); break; case 8:
xprintf(buf2, "Tiger"); break; case 9:
xprintf(buf2, "Monkey"); break; case 10:
xprintf(buf2, "Swallow"); break; case 11:
xprintf(buf2, "Wolf"); break; case 13:
xprintf(buf2, "SS1"); break; case 14:
xprintf(buf2, "SS2"); break; case 15:
xprintf(buf2, "SS3"); break; case 16:
xprintf(buf2, "Pizza"); break; case 17:
xprintf(buf2, "Zarius"); break; default:
                            xprintf(buf2, "N/A"); break;}
i = buf2; break; case 'A':
                            if (!ch->fighting) xprintf(buf2, "N/A");
                            else
                            {
                            xprintf(buf2, "*");
                            if (ch->fighting->max_hit != 0)
                            {
                            CHAR_DATA * och = ch->fighting;
                            int k; for (k = 1; k < 10; k++)
                            {
                            if (och->hit > (och->max_hit * k / 10))
                            strcat(buf2, "*");
                            else
                            strcat(buf2, " ");}
                            }
                            else
                            strcat(buf2, "         ");}
i = buf2; break; case 'a':
                            if (!ch->fighting) xprintf(buf2, "N/A");
                            else
                            {
                            if ((ch->fighting->hit * 100 /
                                 ch->fighting->max_hit) < 25) xprintf(buf2,
                                                                      "#R*");
                            else
                            if ((ch->fighting->hit * 100 /
                                 ch->fighting->max_hit) < 50) xprintf(buf2,
                                                                      "#L*");
                            else
                            if ((ch->fighting->hit * 100 /
                                 ch->fighting->max_hit) < 75) xprintf(buf2,
                                                                      "#G*");
                            else
                            if ((ch->fighting->hit * 100 /
                                 ch->fighting->max_hit) < 100) xprintf(buf2,
                                                                       "#Y*");
                            else
                            xprintf(buf2, "#C*");
                            if (ch->fighting->max_hit != 0)
                            {
                            CHAR_DATA * och = ch->fighting;
                            int k; for (k = 1; k < 10; k++)
                            {
                            if (och->hit > (och->max_hit * k / 10))
                            strcat(buf2, "*");
                            else
                            strcat(buf2, " ");}
                            strcat(buf2, "#n");}
                            else
                            strcat(buf2, "         ");}
i = buf2; break; case 'c':
                            if (IS_CLASS(ch, CLASS_FAE) && !IS_NPC(ch))
                            {
                            xprintf(buf2, "%d",
                                    ch->pcdata->powers[FAE_ENERGY_TICK]);}
                            else
                            if (IS_CLASS(ch, CLASS_NINJA) && !IS_NPC(ch))
                            {
                            if (IS_SET
                                (ch->pcdata->powers[SHADOW_BITS],
                                 NPOWER_BLOODRAGE)) xprintf(buf2, "on");
                            else
                            xprintf(buf2, "off"); i = buf2; break;}
                            else
xprintf(buf2, "N/A"); i = buf2; break; case 'C':
                            if (IS_CLASS(ch, CLASS_FAE) && !IS_NPC(ch))
                            {
                            xprintf(buf2, "%d",
                                    ch->pcdata->powers[FAE_WILL_TICK]);}
                            else
                            if (IS_CLASS(ch, CLASS_SHADOW) && !IS_NPC(ch))
                            {
                            if (IS_SET
                                (ch->pcdata->powers[SHADOW_BITS],
                                 NPOWER_SHADOWFORM)) xprintf(buf2, "on");
                            else
                            xprintf(buf2, "off"); i = buf2; break;}
                            else
xprintf(buf2, "N/A"); i = buf2; break; case 'k':
                            if (IS_CLASS(ch, CLASS_SHAPESHIFTER)
                                && !IS_NPC(ch))
                            {
                            xprintf(buf2, "%d",
                                    ch->pcdata->powers[SHAPE_COUNTER]);
                            ADD_COLOUR(ch, buf2, L_CYAN); i = buf2; break;}
                            else
                            if (IS_CLASS(ch, CLASS_FAE) && !IS_NPC(ch))
                            {
                            xprintf(buf2, "%d",
                                    ch->pcdata->powers[FAE_PLASMA_TICK]);
                            i = buf2; break;}
                            else
                            if (IS_CLASS(ch, CLASS_DRONE) && !IS_NPC(ch))
                            {
                            xprintf(buf2, "%s",
                                    TIME_UP(ch,
                                            TIMER_DRONE_DISTORT) ? "ready" :
                                    "charging"); i = buf2; break;}
                            else
                            if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT)
                                && !IS_NPC(ch))
                            {
                            xprintf(buf2, "%s",
                                    (ch->pcdata->powers[POWER_TICK] ==
                                     0) ? "ready" : "charging"); i = buf2;
                            break;}
                            else
                            if (IS_CLASS(ch, CLASS_HOBBIT) && !IS_NPC(ch))
                            {
                            if ((obj = get_eq_char(ch, WEAR_SPECIAL)))
                            {
                            if (obj->item_type != ITEM_COOKINGPOT)
                            {
                            xprintf(buf2, "No Pot");}
                            else
                            {
                            xprintf(buf2, "%s", flag_string(hobbit_dishes,
                                                            obj->value[0] +
                                                            obj->value[1] +
                                                            obj->value[2] +
                                                            obj->value[3]));}
                            }
                            else
                            xprintf(buf2, "No Pot"); i = buf2; break;}
                            else
                            if (IS_CLASS(ch, CLASS_MONK) && !IS_NPC(ch))
                            {
                            xprintf(buf2, "%s",
                                    flag_string(monk_combos, ch->monkcrap));
                            i = buf2; break;}
                            else
                            if (IS_CLASS(ch, CLASS_LICH) && !IS_NPC(ch))
                            {
                            for (tank = ch->in_room->people; tank;
                                 tank = tank->next_in_room)
                            {
                            if (!IS_NPC(tank)) continue;
                            if (tank->pIndexData->vnum < MOB_VNUM_FIRE
                                || tank->pIndexData->vnum >
                                MOB_VNUM_CLAY) continue;
                            if (is_same_group(tank, ch)) members++;}
                            xprintf(buf2, "%d", members); i = buf2; break;}
                            else
                            if (IS_CLASS(ch, CLASS_SAMURAI) && !IS_NPC(ch))
                            {
                            xprintf(buf2, "%d",
                                    ch->pcdata->powers[SAMURAI_FOCUS]);
                            i = buf2; break;}
                            else
                            if (IS_CLASS(ch, CLASS_SHADOW) && !IS_NPC(ch))
                            {
                            if (IS_SET
                                (ch->pcdata->powers[SHADOW_BITS],
                                 NPOWER_KNIFESHIELD)) xprintf(buf2, "on");
                            else
                            xprintf(buf2, "off"); i = buf2; break;}
                            else
                            {
                            xprintf(buf2, "N/A"); i = buf2; break;}
case 'K':
                            if (IS_CLASS(ch, CLASS_DROID) && !IS_NPC(ch))
                            {
                            xprintf(buf2, "%d",
                                    ch->pcdata->powers[DROID_ROCKET]);
                            i = buf2; break;}
                            else
                            if (IS_CLASS(ch, CLASS_FAE) && !IS_NPC(ch))
                            {
                            xprintf(buf2, "%d",
                                    ch->pcdata->powers[FAE_MATTER_TICK]);
                            i = buf2; break;}
                            else
                            if (IS_CLASS(ch, CLASS_SHADOW) && !IS_NPC(ch))
                            {
                            if (IS_SET
                                (ch->pcdata->powers[SHADOW_BITS],
                                 NPOWER_BLUR)) xprintf(buf2, "on");
                            else
                            xprintf(buf2, "off"); i = buf2; break;}
                            else
                            {
                            xprintf(buf2, "N/A"); i = buf2; break;}
case 'y':
                            if (IS_CLASS(ch, CLASS_FAE) && !IS_NPC(ch))
                            {
                            xprintf(buf2, "%d",
                                    ch->pcdata->powers[FAE_SHIELD]);}
                            else
xprintf(buf2, "N/A"); i = buf2; break; case 'Y':   // not used yet
xprintf(buf2, "N/A"); i = buf2; break; case 'r':
                            if (ch->in_room)
                            xprintf(buf2, "%s", ch->in_room->name);
                            else
xprintf(buf2, " "); ADD_COLOUR(ch, buf2, L_CYAN); i = buf2; break; case 'R':
                            if (!IS_NPC(ch) && (IS_CLASS(ch, CLASS_WEREWOLF)
                                                || IS_CLASS(ch, CLASS_VAMPIRE)
                                                || IS_CLASS(ch, CLASS_SHADOW)
                                                || IS_CLASS(ch, CLASS_NINJA)
                                                || IS_CLASS(ch, CLASS_DEMON)))
                            {
                            xprintf(buf2, "%d", ch->rage);
                            ADD_COLOUR(ch, buf2, D_RED);}
                            else
xprintf(buf2, "0"); i = buf2; break; case 'b':
xprintf(buf2, "%d", ch->beast); ADD_COLOUR(ch, buf2, L_CYAN); i = buf2; break; case 'B':
                            if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE))
                            {
                            xprintf(buf2, "%d",
                                    ch->pcdata->condition[COND_THIRST]);
                            ADD_COLOUR(ch, buf2, D_RED);}
                            else
                            if (!IS_NPC(ch)
                                && IS_CLASS(ch, CLASS_SHAPESHIFTER))
                            {
                            xprintf(buf2, "%d",
                                    ch->pcdata->condition[COND_FULL]);
                            ADD_COLOUR(ch, buf2, D_RED);}
                            else
xprintf(buf2, "0"); i = buf2; break; case 'p':
xprintf(buf2, "%d", char_hitroll(ch)); COL_SCALE(buf2, ch, char_hitroll(ch), 200); i = buf2; break; case 'P':
xprintf(buf2, "%d", char_damroll(ch)); COL_SCALE(buf2, ch, char_damroll(ch), 200); i = buf2; break; case '%':
                            xprintf(buf2, "%%"); i = buf2; break;}
                            ++str; while ((*point = *i) != '\0')++ point, ++i;}
                            write_to_buffer(d, buf, point - buf); return;}


/*
 * Macintosh support functions.
 */
#if defined(macintosh)
                            int gettimeofday(struct timeval *tp, void *tzp)
                            {
                            tp->tv_sec = time(NULL); tp->tv_usec = 0;}
#endif
/*

int len(char s)
{
	int l = 0;
	int x;

	if (s[0] == '\0') return 0;

	for (x = 0; x < length(s); x++)
	{
		if (s[x] == '#')
		{
			if s[x+1
		}

	}

	return l;
}
*/




/*
 * Send a page to one char.
 */
                            void pager_to_char(const char *txt,
                                               CHAR_DATA * ch)
                            {
                            if (IS_NPC(ch))
                            {
                            send_to_char(txt, ch); return;}



                            if (txt == NULL || ch == NULL || ch->desc == NULL)
                            return; if ((ch->pcdata->pagelen == 0))
                            {
                            send_to_char(txt, ch); return;}

                            ch->desc->showstr_head =
                            alloc_mem(strlen(txt) + 1);
                            strcpy(ch->desc->showstr_head, txt);
                            ch->desc->showstr_point = ch->desc->showstr_head;
                            show_string(ch->desc, "");}

/* string pager 
void show_string_new(struct descriptor_data *d, char *input)
{
	char buffer[4*MAX_STRING_LENGTH];
	char buf[MAX_INPUT_LENGTH];
	register char *scan, *chk;
	int lines = 0, toggle = 1;
	int show_lines;

	one_argument(input,buf);
	if (buf[0] != '\0')
	{
		if (d->showstr_head)
		{
			free_mem(d->showstr_head,STRLEN(d->showstr_head));
			d->showstr_head = 0;
		}
		d->showstr_point  = 0;
		return;
	}

	if (d->character)
		show_lines = d->character->lines;
	else
		show_lines = 0;

	for (scan = buffer; ; scan++, d->showstr_point++)
	{
		if (((*scan = *d->showstr_point) == '\n' || *scan == '\r')
			&& (toggle = -toggle) < 0)
			lines++;

		else if (!*scan || (show_lines > 0 && lines >= show_lines))
		{
			*scan = '\0';
			write_to_buffer(d,buffer,STRLEN(buffer));
			for (chk = d->showstr_point; isspace(*chk); chk++);
			{
				if (!*chk)
				{
					if (d->showstr_head)
					{
						free_mem(d->showstr_head,STRLEN(d->showstr_head));
						d->showstr_head = 0;
					}
					d->showstr_point  = 0;
				}
			}
			return;
		}
	}
	return;
}
*/
/* OLC, new pager for editing long descriptions. */
/* ========================================================================= */
/* - The heart of the pager.  Thanks to N'Atas-Ha, ThePrincedom for porting  */
/*   this SillyMud code for MERC 2.0 and laying down the groundwork.         */
/* - Thanks to Blackstar, hopper.cs.uiowa.edu 4000 for which the improvements*/
/*   to the pager was modeled from.  - Kahn                                  */
/* - Safer, allows very large pagelen now, and allows to page while switched */
/*   Zavod of jcowan.reslife.okstate.edu 4000.                               */
/* ========================================================================= */

                            void show_string(DESCRIPTOR_DATA * d, char *input)
                            {
                            char *start, *end;
                            char arg[MAX_INPUT_LENGTH];
                            int lines = 0, pagelen;
                            pagelen =
                            d->original ? d->original->pcdata->pagelen : d->
                            character->pcdata->pagelen;
                            one_argument(input, arg); switch (UPPER(*arg))
                            {

case '\0':
case 'C':
lines = 0; break; case 'B':
lines = -2 * pagelen; break; case 'H':
write_to_buffer(d, "B     - Scroll back one page.\n\r", 0); write_to_buffer(d, "C     - Continue scrolling.\n\r", 0); write_to_buffer(d, "H     - This help menu.\n\r", 0); write_to_buffer(d, "R     - Refresh the current page.\n\r", 0); write_to_buffer(d, "Enter - Continue Scrolling.\n\r", 0); return; case 'R':
lines = -1 - pagelen; break; default:
                            free_string(d->showstr_head);
                            d->showstr_head = NULL;
                            d->showstr_point = NULL; return;}

                            if (lines < 0)
                            {
                            for (start = d->showstr_point;
                                 start > d->showstr_head && lines < 0;
                                 start--) if (*start == '\r') lines++;}
                            else
                            start = d->showstr_point;
                            lines = 0;
                            for (end = start; *end && lines < pagelen; end++)
                            if (*end == '\r')
                            lines++;
                            d->showstr_point = end;
                            if (end - start)
                            write_to_buffer(d, start, end - start);
                            for (; isspace(*end); end++); if (!*end)
                            {
                            free_string(d->showstr_head);
                            d->showstr_head = NULL; d->showstr_point = NULL;}

                            return;}


#if defined( WIN32 )
                            void gettimeofday(struct timeval *tp, void *tzp)
                            {
                            tp->tv_sec = time(NULL); tp->tv_usec = 0;}
#endif

                            void xlogf(char *fmt, ...)
                            {
                            char buf[2 * MSL];
                            va_list args;
                            va_start(args, fmt);
                            vsprintf(buf, fmt, args);
                            va_end(args); log_string(LOG_BUG, buf);}

                            void ch_printf(CHAR_DATA * ch, char *fmt, ...)
                            {
                            char buf[MAX_STRING_LENGTH];
                            va_list args;
                            va_start(args, fmt);
                            vsprintf(buf, fmt, args);
                            va_end(args); stc(buf, ch); return;}
