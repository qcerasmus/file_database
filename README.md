# File Database

This is just a simple header file database class.
It saves full structures to hard drive and definitely doesn't support strings with dynamic lengths.
You probably don't ever want to use this in production code, but I'm not your father...

## Performance
It will vary wildly! Depending on strucure size, hard drive speed and probably a few other things as well.

On my machine I got the following:
./database_benchmark 
2024-02-17T20:10:59+02:00
Running ./database_benchmark
Run on (24 X 3800.05 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x12)
  L1 Instruction 32 KiB (x12)
  L2 Unified 512 KiB (x12)
  L3 Unified 16384 KiB (x4)
Load Average: 0.24, 0.28, 0.35
-------------------------------------------------------------------------
Benchmark                               Time             CPU   Iterations
-------------------------------------------------------------------------
database_benchmark_insert/100      156931 ns       158649 ns         4437
database_benchmark_insert/200      307918 ns       303777 ns         2322
database_benchmark_insert/300      438868 ns       443684 ns         1568
database_benchmark_insert/400      584124 ns       590698 ns         1191
database_benchmark_insert/500      740013 ns       738947 ns          949
database_benchmark_insert/600      862237 ns       872254 ns          801
database_benchmark_insert/700     1012211 ns      1024147 ns          682
database_benchmark_insert/800     1145529 ns      1158317 ns          606
database_benchmark_insert/900     1340647 ns      1313071 ns          533
database_benchmark_insert/1000    1458216 ns      1459752 ns          482

so basically just over 1.4ms per insert.
