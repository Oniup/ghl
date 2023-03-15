#ifndef __OGL_UTILS_FILE_HPP__
#define __OGL_UTILS_FILE_HPP__

#include "ogl/utils/utils.hpp"

#define OGL_FILE_CANNOT_READ_LENGTH std::numeric_limits<size_t>().max()

namespace ogl {

	enum FileOpenState {
		FileOpenState_Closed = 0,
		FileOpenState_Read,
		FileOpenState_Write,
		FileOpenState_ReadBinary,
		FileOpenState_WriteBinary,
	};

	class File {
	public:
		File() = default;
		File(std::string_view path);
		File(std::string_view path, FileOpenState state);
		File(const File& other);
		File(File&& other);
		File& operator=(const File& other);
		File& operator=(File&& other);
		~File();

		inline bool exists() const { return m_file != nullptr; }
		inline std::FILE* get_c_file(){ return m_file; }
		inline const std::FILE* get_c_file() const { return m_file; }
		inline const std::string get_path() const { return m_path; }
		inline const FileOpenState get_open_state() const { return m_open_state; }

		bool is_readable() const;
		bool is_writeable() const;

		size_t get_length() const;
		std::string get_source() const;
		std::vector<char> get_source_as_vector() const;

		void close();

	private:
		void _open_file();

		std::string m_path{};
		FileOpenState m_open_state{ FileOpenState_Closed };
		std::FILE* m_file{ nullptr };
	};

}

#endif