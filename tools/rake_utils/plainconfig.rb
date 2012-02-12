require 'pp'

class PlainConfig
	def initialize(config)
		@settings = defaults()
		@settings.merge!(config)
		
		# Process Source paths
		@settings[:source_files] = make_file_list( @settings[:source_files] )
		
		# Process Include paths
		@settings[:include_dirs] = make_file_list( @settings[:include_dirs] )
		@settings[:include_dirs] = make_option_list( '-I', @settings[:include_dirs] )

		# Process compiler options
		@settings[:compiler_options] = make_option_list('',@settings[:compiler_options])

		# Process linker options
		@settings[:linker_options] = make_option_list('',@settings[:linker_options])

		# Process preprocessor defines
		@settings[:preprocessor_defines] = make_option_list('-D',@settings[:preprocessor_defines])
	end

	def defaults()
		return {
			:name => 'binary',
			:output_dir => 'build',

			:compiler_bin => 'c++',
			:compiler_options => ['-c'],
			:preprocessor_defines => [],
			
			:linker_bin => 'c++',
			:linker_options => [],
	
			:source_files => [ 'source/*.c*' ],
			:include_dirs => [ 'source/**/' ],		
			:directories => []
		}
	end

	def to_s
		pp @settings
	end
	
	def get_bin_extension()
		if ENV['OS'] == 'Windows_NT' then 
			return '.exe'
		else 
			return ''
		end
	end

	def make_option_list(prefix,list)
		if list != nil then
			return list.collect{|opt| "#{prefix}#{opt} "}
		else
			return list
		end
	end
	
	def make_file_list(patt_list)
		file_list = []
		patt_list.each {|f|
			file_list.concat( FileList[f] )
		}
		return file_list
	end

	def register_directory(dir_name)
		@settings[:directories].push(dir_name)
		directory dir_name
		CLOBBER.include(dir_name)
	end

	def directories()
		return @settings[:directories]
	end

	def compile(input,output)
		config = @settings
		sh "#{config[:compiler_bin]} #{config[:compiler_options]} #{config[:preprocessor_defines]} #{config[:include_dirs]} -o #{output} #{input}"
	end

	def link(*args)
		config = @settings
		if args.size == 2 then
			file_list = args[0]
			output = args[1]
			sh "#{config[:linker_bin]} #{config[:linker_options]} -o #{output} #{file_list.collect{|x| x + ' '}}"
		else
			sh "#{config[:linker_bin]} #{config[:linker_options]} -o #{config[:bin_name]} #{config[:object_files].collect{|x| x + ' '}}"
		end
	end
end
