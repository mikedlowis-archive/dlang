require "#{File.expand_path(File.dirname(__FILE__))}/plainconfig"
require 'rake'
require 'rake/clean'

class TestConfig < PlainConfig
	def initialize(config)
		super(config)

		# Register test output directory
		register_directory("#{@settings[:output_dir]}/test")

		# Process test paths
		@settings[:test_files] = make_file_list( @settings[:test_files] )

		# Create source to runner lookup table
		@settings[:src_test_lookup] = {}
		@settings[:test_files].each{ |test|
			out_dir = "#{@settings[:output_dir]}/test"
			runner = "#{out_dir}/#{File.basename(test).ext('')}_runner.cpp"
			@settings[:src_test_lookup][runner] = test
			@settings[:runners].push( runner.ext(get_bin_extension()) )
		}
	end

	def defaults()
		defs = super()
		defs.merge!({
			:test_gen_bin => 'python tools/cxxtest/bin/cxxtestgen',
			:test_gen_options => ['--error-printer'],
			:output_dir => 'build',
			:test_files => ['tests/source/**.h'],
			:include_dirs => [ '.','tests/**/', 'source/**/', 'tools/cxxtest' ],
			:runners => []
		})
		return defs
	end

	def directories()
		dir_list = [ "#{@settings[:output_dir]}/test" ]
		directory dir_list[0]
		CLOBBER.include(dir_list[0])
		return dir_list
	end

	def runners()
		return @settings[:runners]
	end

	def bin_obj_lookup()
		return lambda{|bin| bin.ext('.o')}
	end

	def obj_src_lookup()
		return lambda{|obj| obj.ext('.cpp')}
	end

	def src_test_lookup()
		return lambda{|src| @settings[:src_test_lookup][src] }
	end

	def generate_test_runner(input,output)
		sh "#{@settings[:test_gen_bin]} #{@settings[:test_gen_options]} -o #{output} #{input}" 
	end

	def run_all_test_runners()
		succeeded = true
		puts ''
		puts '------------------'
		puts '    Unit Tests'
		puts '------------------'
		puts ''
		@settings[:runners].each{|runner|
			status = system(runner)
			succeeded = (succeeded and status)
			puts ''
		}
		if not succeeded then 
			abort('Error: There are unit test failures')
		end
	end
end

