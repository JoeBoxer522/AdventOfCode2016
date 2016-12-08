// AdventOfCode2016.cpp
//

#include "AdventTimer.h"
#include "Days.h"

int main(int argc, const char* argv[])
{
	Day* day = new Day08();

    double runTime = 0.0;
    AdventTimer timer;
	day->RunPart1();
    printf("\nPart 1 runtime: %d\n", timer.current());

    start = HiResClock::now();
	day->RunPart2();
    runTime = chrono::duration<double, std::milli>{ HiResClock::now() - start }.count();
    printf("\nPart 2 runtime: %d\n", runTime);

	delete day;
    system("pause");
	return 0;

    double        resTime{ 0.0 };
    std::ifstream is{ "./inputs/" + asString(DAY) + ".txt" };
    if(time) {
        Timer t;
        solve<DAY>(part2, is, os);
        resTime = t.current();
        os.precision(5);
        os << "  time: ";
        os.setf(std::ios::fixed, std::ios::floatfield);
        os << resTime << "ms" << std::endl;
    }
    else
        solve<DAY>(part2, is, os);
    return resTime;


    std::ofstream DEVNULL{ "/dev/null" };
    options_t     options = parseArgs(argc, argv);
    double        totalTime{ 0.0 };
    std::ostream  os{ options.time == TIME_TOTAL ? DEVNULL.rdbuf() : std::cout.rdbuf() };
    for(int d{ Day01 }; d != TOTAL_DAYS; ++d) 
    {
        Day day{ static_cast<Day>(d) };
        if(!std::regex_search(asString(day), options.filter))
            continue;
        os << asString(day) << ((options.part1 && options.part2) ? "\n" : ": ");
        if(options.part1) {
            if(options.part2)
                os << "Part 1: ";
            totalTime += run(day, false, options.time, os);
        }
        if(options.part2) {
            if(options.part1)
                os << "Part 2: ";
            totalTime += run(day, true, options.time, os);
        }
    }
    if(options.time == TIME_TOTAL) {
        std::cout.precision(5);
        std::cout << "  time: ";
        std::cout.setf(std::ios::fixed, std::ios::floatfield);
        std::cout << totalTime << "ms" << std::endl;
    }
    return EXIT_SUCCESS;
}

parseArgs(int argc, char* argv[])
{
    options_t                options;
    static char const* const shortOpts{ "htnp:f:" };
    static const std::array<const ::option, 6> longOpts{ { { "help", no_argument, nullptr, 'h' },
    { "part", required_argument, nullptr, 'p' },
    { "filter", required_argument, nullptr, 'f' },
    { "time", required_argument, nullptr, 't' },
    { nullptr, 0, nullptr, 0 } } };
    int option{ 0 };
    while(option = getopt_long(argc, argv, shortOpts, longOpts.data(), nullptr), option != -1) {
        switch(option) {
        case 'h':
        case '?':
            printf("Advent of Code - 2015\n"
                "---------------------\n"
                " -h|--help )\n    print help\n"
                " -p|--part=[1,2,all] )\n    only run parts specified [default = all]\n"
                " -f|--filter=<regex> )\n    filter day on regular expression [default = match all]\n"
                " -t|--time=[no,yes,total] )\n print timing of exection [default = no]\n"
                "\n"
                " Implementation by William Killian (c) 2016\n");
            exit(EXIT_SUCCESS);
            break;
        case 'p':
            if(optarg[0] == '1')
                options.part2 = false;
            else if(optarg[0] == '2')
                options.part1 = false;
            break;
        case 'f':
            options.filter = std::regex{ optarg };
            break;
        case 't':
            std::string arg{ optarg };
            options.time = (arg == "no" ? NO_TIME : (arg == "yes" ? TIME_IND : TIME_TOTAL));
            break;
        }
    }
    return options;
}

double timeSolve(bool part2, bool time, std::ostream& os)
{
    double        resTime{ 0.0 };
    std::ifstream is{ "./inputs/" + asString(DAY) + ".txt" };
    if(time) 
    {
        Timer t;
        solve<DAY>(part2, is, os);
        resTime = t.current();
        os.precision(5);
        os << "  time: ";
        os.setf(std::ios::fixed, std::ios::floatfield);
        os << resTime << "ms" << std::endl;
    }
    else
        solve<DAY>(part2, is, os);
    return resTime;
}