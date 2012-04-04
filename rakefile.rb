include Rake::DSL
require 'rake/clean'
require 'tools/rake_utils/source/binary.rb'
require 'tools/rake_utils/source/tests.rb'

# Keep track of the project root
PROJECT_ROOT  = File.expand_path(File.dirname(__FILE__))

# Add library build folders to the clobber list
CLOBBER.include('./deps/cork/build/static')
CLOBBER.include('./deps/cork/build/shared')
CLOBBER.include('./deps/parse-utils/build/static')
CLOBBER.include('./deps/parse-utils/build/shared')
CLOBBER.include('./tools/UnitTest++/*UnitTest++*')
CLOBBER.include('./tools/UnitTest++/src/**/*.o')
CLOBBER.include('./tests/gcov')

#------------------------------------------------------------------------------
# Configuration Objects
#------------------------------------------------------------------------------
# Configuration for the release binary artifact
DLangParser = Binary.new({
    :name => 'dlang',
    :output_dir => 'build/release',
    :compiler_options => [ '-c', '-Wall', '-Werror', '-o' ],
    :static_libs => [
        './deps/parse-utils/build/static/bin/libparse-utils.a',
    ],
    :resource_files => [ 'res/*' ],
    :source_files => [ 'source/**/*.c*' ],
    :include_dirs => [
        'source/**/',
        'deps/parse-utils/source/**/'
    ],
})
DLangParser.setup_default_rake_tasks()

# Configuration for the release binary artifact
DLangDebug = Binary.new({
    :name => 'dlang',
    :output_dir => 'build/debug',
    :compiler_options => [ '-c', '-Wall', '-Werror', '-o' ],
    :static_libs => [
        './deps/cork/build/static/bin/libcork.a',
        './deps/parse-utils/build/static/bin/libparse-utils.a',
    ],
    :preprocessor_defines => [ 'DEBUG' ],
    :resource_files => [ 'res/*' ],
    :source_files => [ 'source/**/*.c*' ],
    :include_dirs => [
        'source/**/',
        'deps/cork/source/**/',
        'deps/parse-utils/source/**/'
    ],
})
DLangDebug.setup_default_rake_tasks()

# Configuration for the unit test runner
DLangTests = Binary.new({
    :name => 'test_runner',
    :output_dir => 'build/test',
    :linker_options => [ '-fprofile-arcs', '-o' ],
    :compiler_options => [
        '-c', '-Wall', '-Werror',
        '-fprofile-arcs', '-ftest-coverage',
        '-O0', '-o'
    ],
    :static_libs => [
        'tools/UnitTest++/libUnitTest++.a',
        './deps/parse-utils/build/static/bin/libparse-utils.a'
    ],
    :resource_files => [ 'res/*' ],
    :source_files => [
        'tests/**/*.c*',
        'source/*/**/*.c*'
    ],
    :include_dirs => [
        'tests/**/',
        'source/**/',
        'tools/UnitTest++/src/**/',
        'deps/parse-utils/source/**/'
    ],
})
DLangTests.setup_default_rake_tasks()

#------------------------------------------------------------------------------
# Main Tasks
#------------------------------------------------------------------------------
desc 'Build all binary artifacts and run all tests'
task :default => [ :test, :debug, :release ]

desc 'Build and link all artifacts'
task :release => [ :parse_utils, DLangParser.name() ]

desc 'Build and link all artifacts with the debug build option'
task :debug => [ :cork, :parse_utils, DLangDebug.name() ]

desc 'Build and run all unit tests'
task :test => [ :parse_utils, :unit_test_pp, DLangTests.name() ] do
    sh 'build/test/bin/test_runner'
end

desc 'Build the cork memory leak detector'
task :cork do
    Dir.chdir('./deps/cork')
    sh 'rake release'
    Dir.chdir(PROJECT_ROOT)
end

desc 'Build the parse-utils library'
task :parse_utils do
    Dir.chdir('./deps/parse-utils')
    sh 'rake release'
    Dir.chdir(PROJECT_ROOT)
end

desc 'Build the UnitTest++ library'
task :unit_test_pp do
    Dir.chdir('./tools/UnitTest++')
    sh 'make all'
    Dir.chdir(PROJECT_ROOT)
end

desc 'Generate and display the test coverage statistics for each test file'
task :coverage do #=> [ :test ] do
    out_dir = 'build/test/gcov'
    tests = FileList['tests/**/test_*.cpp']

    # Create the output directory if it doesnt already exist
    if not File.exist?( out_dir ) then
        Dir.mkdir( out_dir )
    end

    # For each test file
    puts ""
    tests.each { |test|
        # Find the source file basename, object file location, and gcov output
        source = File.basename( test ).gsub( 'test_', '' )
        obj = 'build/obj/' + source.ext('o')
        gcov = source + '.gcov'

        # Generate the coverage info and display only the summary for our
        # source file
        puts "----"
        sh "gcov -b -o build/test/obj #{obj} | grep -A 4 \"#{source}'\""

        # Move the coverage to our output folder
        FileUtils.mv( gcov, out_dir )

        # Delete the unwanted coverage files
        FileList['*.gcov'].each { |f| File.delete(f) }
    }
    puts ""
end
