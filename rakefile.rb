include Rake::DSL
require 'tools/rake_utils/buildconfig.rb'
require 'tools/rake_utils/testconfig.rb'

#------------------------------------------------------------------------------
# Configuration Objects
#------------------------------------------------------------------------------
# Configuration for the binary artifact
Binary = BuildConfig.new({
    :name => 'dlang',
    :compiler_options => [ '-c', '-Wall', '-Werror' ],
    :source_files => [ 'source/**/*.c*' ],
    :include_dirs => [ 'source/**/' ],
    :preprocessor_defines => [ 'DETECT_MEM_LEAKS' ]
})

# Configuration for the unit tests
UnitTest = TestConfig.new({
    :test_files => [ 'tests/source/**.h' ],
})

#------------------------------------------------------------------------------
# Release Tasks
#------------------------------------------------------------------------------
desc 'Execute a complete build including unit tests and binary'
task :default => [ :release ]

desc 'Display build configuration info'
task :config do
    puts 'Release Configuration'
    puts '---------------------'
    puts Binary
    puts ''
    puts 'Unit Test Configuration'
    puts '-----------------------'
    puts UnitTest
end

desc 'Build and link the binary'
task :release => [ Binary.binary_name() ]

task Binary.binary_name() => Binary.directories() + Binary.objects() do
    Binary.link()
end

rule(/obj\/.+.o$/ => Binary.obj_src_lookup()) do |t|
    Binary.compile(t.source,t.name)
end

#------------------------------------------------------------------------------
# Testing Tasks
#------------------------------------------------------------------------------
desc 'Execute all unit tests'
task :test => UnitTest.directories() + UnitTest.runners() do
    UnitTest.run_all_test_runners();
end

rule '_runner.exe' => UnitTest.bin_obj_lookup() do |t|
    UnitTest.link([t.source],t.name)
end

rule( /test\/.+_runner.o$/ => UnitTest.obj_src_lookup() ) do |t|
    UnitTest.compile(t.source,t.name)
end

rule '_runner.cpp' => UnitTest.src_test_lookup() do |t|
    UnitTest.generate_test_runner(t.source,t.name)
end

