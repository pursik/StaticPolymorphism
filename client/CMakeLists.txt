add_executable(client
   "main.cpp"
   "src/TaskFactory.cpp"
   "src/CommandFactory.cpp"  
   "src/ProgramOptions.cpp"
   )
target_include_directories(client PRIVATE "include")
# Link libraries
target_link_libraries(client Boost::program_options cryptopp::cryptopp)

