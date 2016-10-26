#define DEFAULT_PCM_BUFFER_SIZE 16384
#define DEFAULT_WEBSOCKET_SERVER_PORT 8080

#include <string>
#include <fstream>
#include <map>

class Config {
	public:
		void loadFile(const char *filePath);
		void saveFile();

		int get(std::string key, int defaultValue);
		void set(std::string key, int value);
		
		bool has(std::string key);

	private:
		std::map<std::string, int> options;
		char *filePath;
};