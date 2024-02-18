include (FetchContent)
if (${BENCHMARK})
	set (BENCHMARK_ENABLE_TESTING NO)

	FetchContent_Declare(
		googlebenchmark
		GIT_REPOSITORY https://github.com/google/benchmark.git
		GIT_TAG v1.7.1
	)

	FetchContent_MakeAvailable(googlebenchmark)
endif()
if (${TEST})
	FetchContent_Declare(
		googletest
		GIT_REPOSITORY https://github.com/google/googletest.git
		GIT_TAG release-1.12.1
	)
	set (gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(googletest)
endif()
