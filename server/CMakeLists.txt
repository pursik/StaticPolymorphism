add_executable(server
	"main.cpp"
	"src/TaskFactory.cpp" 
	"src/ProgramOptions.cpp" 
	"src/CommandFactory.cpp" 
)
target_include_directories(server PRIVATE "include")

# Link libraries
target_link_libraries(server Boost::program_options cryptopp::cryptopp)

