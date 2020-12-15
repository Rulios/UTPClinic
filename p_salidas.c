#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_BRIGHT_MAGENTA "\x1b[95m"
#define ANSI_COLOR_RESET   "\x1b[0m"


#define print_red(X) printf("%s %s %s", ANSI_COLOR_RED, X, ANSI_COLOR_RESET)
#define print_green(X) printf("%s %s %s", ANSI_COLOR_GREEN, X, ANSI_COLOR_RESET)
#define print_yellow(X) printf("%s %s %s", ANSI_COLOR_YELLOW, X, ANSI_COLOR_RESET)
#define print_blue(X) printf("%s %s %s", ANSI_COLOR_BLUE, X, ANSI_COLOR_RESET)
#define print_magenta(X) printf("%s %s %s", ANSI_COLOR_MAGENTA, X, ANSI_COLOR_RESET)
#define print_cyan(X) printf("%s %s %s", ANSI_COLOR_CYAN, X, ANSI_COLOR_RESET)
#define print_bright_magenta(X) printf("%s %s %s", ANSI_COLOR_BRIGHT_MAGENTA, X, ANSI_COLOR_RESET)
