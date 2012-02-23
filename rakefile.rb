include Rake::DSL
require 'tools/rake_utils/source/binary.rb'
require 'tools/rake_utils/source/tests.rb'

#------------------------------------------------------------------------------
# Configuration Objects
#------------------------------------------------------------------------------
# Configuration for the binary artifact
DLangParser = Binary.new({
    :name => 'dlang',
    :output_dir => 'build/parser'
    :compiler_options => [ '-c', '-Wall', '-Werror', '-o' ],
    :source_files => [ 'source/**/*.c*' ],
    :include_dirs => [ 'source/**/' ],
    :preprocessor_defines => [ 'DETECT_MEM_LEAKS' ]
})
DLangParser.setup_default_rake_tasks()


# Configuration for the unit tests
UnitTest = Tests.new({
    :test_files => [ 'tests/source/**.h' ],
})
UnitTest.setup_default_rake_tasks()

desc 'Build and link all artifacts'
task :release => [ DLangParser.name() ]
