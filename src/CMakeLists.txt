file(GLOB pathsearch_src *.cc *.cxx *.cpp *.c)

set (CMAKE_CXX_FLAGS "-pg -Wall -Wextra -Wabi-tag -Wcast-align -Wc++0x-compat -Wc++14-compat -Wno-cast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wconditionally-supported -Wconversion-null -Wctor-dtor-privacy -Wredundant-decls -Wdelete-non-virtual-dtor -Wdelete-incomplete -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused -Weffc++ -Winherited-variadic-ctor -Winvalid-offsetof -Wliteral-suffix -Wnoexcept -Wnon-template-friend -Wnon-virtual-dtor -Wno-old-style-cast -Woverloaded-virtual -Wpmf-conversions -Wreorder -Wsign-promo -Wsized-deallocation -Wstrict-null-sentinel -Wno-suggest-override -Wsynth -Wno-useless-cast -Wvirtual-move-assign -Wzero-as-null-pointer-constant ")
set (CMAKE_C_FLAGS "-pg -Waddress -Waggregate-return -Waggressive-loop-optimizations -Wno-long-long -Warray-bounds -Wattributes -Wbad-function-cast -Wbool-compare -Wbuiltin-macro-redefined -Wc90-c99-compat -Wc99-c11-compat -Wcast-align -Wcast-qual -Wchar-subscripts -Wclobbered -Wcomment -Wconversion -Wcoverage-mismatch -Wcpp -Wdate-time -Wdeclaration-after-statement -Wdeprecated -Wdeprecated-declarations -Wdesignated-init -Wdisabled-optimization -Wdiscarded-array-qualifiers -Wdiscarded-qualifiers -Wdiv-by-zero -Wdouble-promotion -Wempty-body -Wendif-labels -Wenum-compare -Wextra -Wfloat-equal -Wformat-contains-nul -Wformat-extra-args -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat-y2k -Wformat-zero-length -Wfree-nonheap-object -Wignored-qualifiers -Wimplicit -Wimplicit-function-declaration -Wimplicit-int -Wincompatible-pointer-types -Winit-self -Winline -Wint-conversion -Wint-to-pointer-cast -Winvalid-memory-model -Winvalid-pch -Wjump-misses-init -Wlogical-not-parentheses -Wlogical-op -Wmain -Wmaybe-uninitialized -Wmemset-transposed-args -Wmissing-braces -Wmissing-field-initializers -Wmissing-include-dirs -Wmissing-parameter-type -Wmultichar -Wnarrowing -Wnested-externs -Wnonnull -Wodr -Wold-style-declaration -Wold-style-definition -Wopenmp-simd -Woverflow -Woverlength-strings -Woverride-init -Wpacked -Wpacked-bitfield-compat -Wparentheses -Wpointer-arith -Wpointer-sign -Wpointer-to-int-cast -Wpragmas -Wredundant-decls -Wreturn-local-addr -Wreturn-type -Wsequence-point -Wshadow -Wshift-count-negative -Wshift-count-overflow -Wsign-compare -Wsizeof-array-argument -Wsizeof-pointer-memaccess -Wstack-protector -Wstrict-prototypes -Wsuggest-attribute=const -Wsuggest-attribute=format -Wsuggest-attribute=noreturn -Wsuggest-attribute=pure -Wsuggest-final-methods -Wsuggest-final-types -Wswitch -Wswitch-bool -Wswitch-default -Wswitch-enum -Wsync-nand -Wsystem-headers -Wtrampolines -Wtrigraphs -Wtype-limits -Wundef -Wuninitialized -Wunknown-pragmas -Wunsafe-loop-optimizations -Wunsuffixed-float-constants -Wunused -Wunused-but-set-parameter -Wunused-but-set-variable -Wunused-function -Wunused-label -Wunused-local-typedefs -Wunused-macros -Wunused-parameter -Wunused-result -Wunused-value -Wunused-variable -Wvarargs -Wvariadic-macros -Wvector-operation-performance -Wvla -Wvolatile-register-var -Wwrite-strings")

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

add_executable(pathsearch ${pathsearch_src})

target_include_directories(pathsearch PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_custom_target(run)
add_custom_command(
    TARGET run POST_BUILD
    COMMAND ./pathsearch
)

add_custom_target(debug)
add_custom_command(
    TARGET debug POST_BUILD
    COMMAND gdb ./pathsearch
)

