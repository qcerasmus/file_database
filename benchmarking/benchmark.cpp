#include <benchmark/benchmark.h>

#include <random>

#include "database.h"
#include "test_struct.h"

std::string generateRandomFileName()
{
    std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

    std::random_device rd;
    std::mt19937 generator(rd());

    std::ranges::shuffle(str, generator);
    return str.substr(0, 64);
}

static void database_benchmark_insert(benchmark::State &state)
{
    for ([[maybe_unused]] auto _ : state)
    {
        database<test> db(generateRandomFileName());
        for (int i = 1; i <= state.range(); i++)
        {
            state.PauseTiming();
            test t;
            t.first = i;
            t.second = i + i;
            for (int j = 0; j < 10; j++)
                t.test_string[j] = 'a' + j;
            state.ResumeTiming();
            auto id = db.AddObject(t);
        }
    }
}
BENCHMARK(database_benchmark_insert)->DenseRange(100, 1000, 100);

BENCHMARK_MAIN();
