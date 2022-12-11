#include "catch.hpp"
#include "IMDBData.h"
#include <sstream>

// Helper function declarations (don't change this)
extern bool CheckTextFilesSame(const std::string& fileNameA,
	const std::string& fileNameB);

// Your tests -- only add sections
TEST_CASE("Student tests", "[student]") 
{
    SECTION("Constructor")
    {
        // try passing in a false file name, invalid argument should be thrown
        try
        {
            IMDBData data("wrongName.list");
            REQUIRE(false);
        }
        catch (std::invalid_argument& e)
        {
            REQUIRE(true);
        }
        catch (std::exception& e)
        {
            REQUIRE(false);
        }
    }
    
    SECTION("Actors to moives")
    {
        // initialize a IMDBData object
        IMDBData data("input/top250.list");
        // get moveis from an actor
        std::vector<std::string> answer = data.getMoviesFromActor("Al Pacino");
        // check if the number of movies is correct
        REQUIRE(answer.size() == 6);
        // check if the movies are in the vector
        REQUIRE(std::find(answer.begin(), answer.end(), "Dog Day Afternoon (1975)") != answer.end());
        REQUIRE(std::find(answer.begin(), answer.end(), "The Godfather: Part II (1974)") != answer.end());
        
        // test another actor who is not in the list
        answer = data.getActorsFromMovie("Bob Jarvis");
        REQUIRE(answer.size() == 0);
    
    }
    
    SECTION("Movies to Actors")
    {
        // initialize a IMDBData object
        IMDBData data("input/top250.list");
        // get a list of actors from a movie
        std::vector<std::string> answer = data.getActorsFromMovie("Jurassic Park (1993)");
        // check if the number of actors is correct
        REQUIRE(answer.size() == 29);
        // chekc if a few actors are in the vector
        REQUIRE(std::find(answer.begin(), answer.end(), "Dean Cundey") != answer.end());
        REQUIRE(std::find(answer.begin(), answer.end(), "Samuel L. Jackson") != answer.end());
    }
    
    SECTION("Find Relationship")
    {
        // initialize a data object and its associated graph
        IMDBData data("input/top250.list");
        data.createGraph();
        // check for both actors unknown
        std::string answer = "Bob Jarvis is unknown!\nAaron Gong is unknown!\n";
        std::string output = data.findRelationship("Bob Jarvis", "Aaron Gong");
        REQUIRE(answer == output);
        // check when one actor is unknown
        answer = "Bob Jarvis is unknown!\n";
        output = data.findRelationship("Bob Jarvis", "Dean Cundey");
        REQUIRE(answer == output);
        // check several relationships
        output = data.findRelationship("Judy Garland", "Al Pacino");
        // can only check if the number of hops are correct
        std::stringstream outStream(output);
        std::string hopString;
        getline(outStream, hopString);
        REQUIRE(hopString == "Found a path! (3 hops)");
        // check another relationship
        output = data.findRelationship("Gail Yudain","Seguna");
        std::stringstream outStream2(output);
        getline(outStream2, hopString);
        REQUIRE(hopString == "Found a path! (6 hops)");
        // test two actors who do not have connections
        answer = "Didn't find a path.\n";
        output = data.findRelationship("Sahar Kave", "Seguna");
        REQUIRE(answer == output);
    }
}


