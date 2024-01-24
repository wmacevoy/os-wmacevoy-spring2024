# Use an official GCC runtime as a parent image
FROM gcc:latest

# Set the working directory in the container
WORKDIR /usr/src/app

# Copy the current directory contents into the container at /usr/src/app
COPY . .

# Compile the C++ program
RUN g++ -o HelloWorld main.cpp

# Run the program when the container launches
CMD ["./HelloWorld"]
