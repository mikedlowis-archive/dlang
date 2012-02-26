include Rake::DSL
require 'tools/rake_utils/source/binary.rb'
require 'tools/rake_utils/source/tests.rb'

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
    #:preprocessor_defines => [ 'DETECT_MEM_LEAKS' ]
})
DLangParser.setup_default_rake_tasks()


# Configuration for the unit tests
UnitTest = Tests.new({
    :test_files => [ 'tests/source/**.h' ],
})
UnitTest.setup_default_rake_tasks()

desc 'Build and link all artifacts'
task :release => [ DLangParser.name() ]
