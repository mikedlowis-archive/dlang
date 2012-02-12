require "#{File.expand_path(File.dirname(__FILE__))}/plainconfig"
require 'rake'
require 'rake/clean'

class BuildConfig < PlainConfig
	def initialize(config)
		super(config)

		# Register output directories
		register_directory("#{@settings[:output_dir]}/bin")
		register_directory("#{@settings[:output_dir]}/obj")
		register_directory("#{@settings[:output_dir]}/test")

		# set output name
		@settings[:bin_name] = "#{@settings[:output_dir]}/bin/#{@settings[:name]}#{get_bin_extension()}"
		CLEAN.include(@settings[:bin_name])

		# Create object file list
		@settings[:object_source_lookup] = {}
		@settings[:object_files] = []
		@settings[:source_files].each{ |f|
			obj_file = @settings[:output_dir] + '/obj/' + File.basename(f).ext('o')
			CLEAN.include(obj_file)
			@settings[:object_files].push( obj_file )
			@settings[:object_source_lookup][obj_file] = f
		}
	end

	def binary_name()
		return @settings[:bin_name]
	end
	
	def objects()
		return @settings[:object_files]
	end

	def source_from_obj(obj)
		return @settings[:object_source_lookup][obj]
	end

	def obj_src_lookup()
		return lambda{|obj| @settings[:object_source_lookup][obj]}
	end
end
