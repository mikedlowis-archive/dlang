include Rake::DSL
require 'rake/clean'
require 'tools/rake_utils/source/binary.rb'
require 'tools/rake_utils/source/tests.rb'

PROJECT_ROOT  = File.expand_path(File.dirname(__FILE__))
CLOBBER.include('./deps/cork/build/static')
CLOBBER.include('./deps/cork/build/shared')

#------------------------------------------------------------------------------
# Configuration Objects
#------------------------------------------------------------------------------
# Configuration for the binary artifact
DLangParser = Binary.new({
    :name => 'dlang',
    :output_dir => 'build/parser',
    :compiler_options => [ '-c', '-Wall', '-Werror', '-o' ],
    :static_libs => [ './deps/cork/build/static/bin/libcork.a' ],
    :source_files => [ 'source/**/*.c*' ],
    :include_dirs => [
        'source/**/',
        'deps/cork/source/**/'
    ],
})
DLangParser.setup_default_rake_tasks()


# Configuration for the unit tests
UnitTest = Tests.new({
    :test_files => [ 'tests/source/**.h' ],
})
UnitTest.setup_default_rake_tasks()

desc 'Build and link all artifacts'
task :release => [ :cork, DLangParser.name() ]

task :cork do
    Dir.chdir('./deps/cork')
    sh 'rake release'
    Dir.chdir(PROJECT_ROOT)
end
