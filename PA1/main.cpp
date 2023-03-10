#include <string>
#include <iostream>
#include <filesystem>
struct SharedResource //A shared resource among threads
{
	typedef std::vector<std::vector<std::filesystem::directory_entry>> vectDirFile;
public:
	SharedResource() :
		mtx()
	{
	}
	vectDirFile resultVect;
	std::mutex mtx;
};
typedef std::vector<std::filesystem::directory_entry> vectDirEntry;

void ListFiles(std::filesystem::path&& dir_path, SharedResource& sharedResource)
{
	vectDirEntry vect; //local variables are not shared between all the threads
	for (const auto& entry : std::filesystem::directory_iterator(dir_path))
	{
		if (entry.is_directory())
		{

			vect.push_back(entry);
			auto result = std::async(std::launch::async, &ListFiles, (std::filesystem::path)entry, std::ref(sharedResource));

		}
		else if (entry.is_regular_file())
		{

			vect.push_back(entry);
		}
	}
	std::unique_lock<std::mutex> lock;

	if (!vect.empty())
		sharedResource.resultVect.push_back(vect); //This is a shared resource so we should protect it from the race condition
	lock.unlock();

}
int main()
{
	START_BANNER_MAIN("--Main--");
	const std::string dir_path = "C:/Program Files (x86)/Windows Kits";
	SharedResource sharedResource;
	auto result = std::async(std::launch::async, &ListFiles, dir_path, std::ref(sharedResource));
	result.wait();
	std::cout << "-------------------------------------\n";
	int counter = 0;
	for (std::vector<std::filesystem::directory_entry>& item : sharedResource.resultVect)
	{

		std::for_each(item.begin(), item.end(),
			[&](std::filesystem::directory_entry& entry)


			{
				counter++;
				std::cout << entry << '\n';

			});
	}


}

