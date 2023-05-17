#include <chrono>
#include <cstdio>


#define TEST(condition, message)                                                                 \
    (!(condition)) ? (printf(ERROR "╔[ TEST ]=(%s)=[%s line %d] \n╚> " WHITE("%s") "\n" RESET,   \
                             QUOTE(condition), __FILE__, __LINE__, message),0)                    \
                   : (printf(PASS "╔[ TEST ]=(%s)=[%s line %d] \n╚> " WHITE("%s") "\n" RESET,   \
                             QUOTE(condition), __FILE__, __LINE__, message),1);

#define RESET "\033[0m"
#define IGNORE_UNUSED(x) (void) (x)

#define GREEN(x) "\033[1;32m" x
#define YELLOW(x) "\033[1;33m" x
#define RED(x) "\033[1;31m" x
#define WHITE(x) "\033[1;37m" x

#define ERROR "\033[1;31m"
#define WARN "\033[1;33m"
#define PASS "\033[1;32m"

#define QUOTE(str) #str

#define EXPAND_AND_QUOTE(str) QUOTE(str)

#ifdef NDEBUG
#define ASSERT(condition, message)
#define ASSERT_V(condition, message)
#define ASSERT_S(condition, message)
#define TIMEIT(function) function

#else

#define ASSERT_V(condition, format, ...)                                                           \
    (!(condition)) ? (printf(ERROR "╔[ ASSERT ]=(%s)=[%s line %d] \n╚> " WHITE(format) "\n" RESET, \
                             QUOTE(condition), __FILE__, __LINE__, __VA_ARGS__),                   \
                      exit(1), 0)                                                                  \
                   : 1;

#define ASSERT(condition, message)                                                                 \
    (!(condition)) ? (printf(ERROR "╔[ ASSERT ]=(%s)=[%s line %d] \n╚> " WHITE("%s") "\n" RESET,   \
                             QUOTE(condition), __FILE__, __LINE__, message),                       \
                      exit(1), 0)                                                                  \
                   : 1;

#define ASSERT_S(condition)                                                                        \
    (!(condition)) ? (printf(ERROR "╔[ ASSERT ]=(%s)=[%s line %d] \n" RESET, QUOTE(condition),     \
                             __FILE__, __LINE__),                                                  \
                      exit(1), 0)                                                                  \
                   : 1;

#define INVOKE(f,...) if (f) {f(__VA_ARGS__) ;}


#endif

#ifdef NLOG

#define LOG(format, data)
#define LOG_V(status, format, ...)
#define DPRINT(var)
#define TIMEIT(function, message)                                                                  

#else

#define LOG_V(status, format, ...)                                                                 \
    printf(status "╔[ LOG ]=[%s line %d] \n╚> " WHITE(format) "\n" RESET, __FILE__, __LINE__,      \
           __VA_ARGS__);

#define LOG(status, message)                                                                       \
    printf(status "╔[ LOG ]=[%s line %d] \n╚> " WHITE("%s") "\n" RESET, __FILE__, __LINE__,        \
           message);

#define DPRINT(var)\
    std::cout << #var << "=" << var << std::endl;

#define TIMEIT(function, message)                                                                  \
    {                                                                                              \
        std::chrono::time_point<std::chrono::system_clock> _start, _end;                           \
        _start = std::chrono::system_clock::now();                                                 \
        function;                                                                                  \
        _end = std::chrono::system_clock::now();                                                   \
        std::chrono::duration<double> _elapsed_seconds = _end - _start;                            \
        printf("=[ TIME ]=(" WHITE("%s") ") == > % f's\n", message, _elapsed_seconds.count());     \
    }

#endif
// Regular Colors
// Value	Color
// \e[0;30m	Black
// \e[0;31m	Red
// \e[0;32m	Green
// \e[0;33m	Yellow
// \e[0;34m	Blue
// \e[0;35m	Purple
// \e[0;36m	Cyan
// \e[0;37m	White
// Bold
// Value	Color
// \e[1;30m	Black
// \e[1;31m	Red
// \e[1;32m	Green
// \e[1;33m	Yellow
// \e[1;34m	Blue
// \e[1;35m	Purple
// \e[1;36m	Cyan
// \e[1;37m	White
// Underline
// Value	Color
// \e[4;30m	Black
// \e[4;31m	Red
// \e[4;32m	Green
// \e[4;33m	Yellow
// \e[4;34m	Blue
// \e[4;35m	Purple
// \e[4;36m	Cyan
// \e[4;37m	White
