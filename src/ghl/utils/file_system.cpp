#include "ghl/utils/file_system.hpp"
#include "ghl/core/debug.hpp"

namespace ghl {

	File::File(std::string_view path) : File(path, FileOpenState_Read) {
	}

	File::File(std::string_view path, FileOpenState state) : m_path(path), m_open_state(state) {
		_open_file();
	}

	File::File(const File& other) {
		*this = File(other.m_path, other.m_open_state);
	}

	File::File(File&& other) {
		*this = File(std::move(other));
	}

	File& File::operator=(const File& other) {
		close();

		m_path = other.m_path;
		m_open_state = other.m_open_state;

		_open_file();
		return *this;
	}

	File& File::operator=(File&& other) {
		close();

		m_path = std::move(other.m_path);
		m_open_state = other.m_open_state;
		m_file = other.m_file;

		other.m_open_state = FileOpenState_Closed;
		m_file = nullptr;

		return *this;
	}

	File::~File() {
		close();
	}

	size_t File::get_length() const {
		if (is_readable()) {
			std::fseek(m_file, 0, SEEK_END);
			size_t length = static_cast<size_t>(std::ftell(m_file));
			std::fseek(m_file, 0, SEEK_SET);

			return length;
		}

		return GHL_FILE_CANNOT_READ_LENGTH;
	}

	bool File::is_readable() const {
		if (m_file != nullptr) {
			if (m_open_state == FileOpenState_Read || m_open_state == FileOpenState_ReadBinary) {
				return true;
			}
		}

		return false;
	}

	bool File::is_writeable() const {
		if (m_file != nullptr) {
			if (m_open_state == FileOpenState_Write || m_open_state == FileOpenState_WriteBinary) {
				return true;
			}
		}

		return false;
	}

	std::string File::get_source() const {
		return std::string(get_source_as_vector().data());
	}

	std::vector<char> File::get_source_as_vector() const {
		if (is_readable()) {
			size_t length = get_length();
			std::vector<char> source(length + 1);

			std::fread(source.data(), length, sizeof(char), m_file);
			source[length] = '\0';
			return source;
		}

		return {};
	}

	void File::close() {
		if (m_file != nullptr) {
			std::fclose(m_file);
			m_file = nullptr;
		}
	}

	void File::_open_file() {
		close();

		char state[3] = { ' ', ' ', '\0' };
		switch (m_open_state) {
		case FileOpenState_Closed:
			return;
		case FileOpenState_Read:
			state[0] = 'r';
			break;
		case FileOpenState_Write:
			state[0] = 'w';
			break;
		case FileOpenState_ReadBinary:
			state[0] = 'r';
			state[1] = 'b';
			break;
		case FileOpenState_WriteBinary:
			state[0] = 'w';
			state[1] = 'b';
			break;
		}

		m_file = std::fopen(m_path.c_str(), state);
		if (m_file == nullptr) {
			Debug::log("failed to load file \"" + m_path + "\" as it doesn't exit");
			return;
		}
	}

}