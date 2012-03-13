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

# Configuration for the unit tests
UnitTest = Tests.new({
    :test_files => [ 'tests/source/**.h' ],
})
UnitTest.setup_default_rake_tasks()

#------------------------------------------------------------------------------
# Main Tasks
#------------------------------------------------------------------------------
desc 'Build and link all artifacts'
task :release => [ :parse_utils, DLangParser.name() ]
task :debug => [ :cork, :parse_utils, DLangDebug.name() ]

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
