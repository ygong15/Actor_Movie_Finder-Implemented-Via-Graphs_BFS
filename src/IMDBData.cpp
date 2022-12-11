#include "IMDBData.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>

IMDBData::IMDBData(const std::string& fileName)
{
    std::ifstream inFile(fileName);
    if(!inFile.is_open())
    {
        throw std::invalid_argument("file cannot be opend");
    }
    std::string inLine = "";
    std::string inActor = "";
    std::string inMovie = "";
    while(!inFile.eof())
    {
        getline(inFile, inLine);
        // if the line is an actor's name
        if(inLine[0] != '|')
        {
            inActor = inLine;
            // create a new entry in the hash map
            mActorsToMoviesMap[inActor] = std::vector<std::string>();
        }
        // read in a movie
        else
        {
            inMovie = inLine.substr(1, inLine.length());
            mActorsToMoviesMap[inActor].push_back(inMovie);
        }
        
    }
    for(const auto& itr : mActorsToMoviesMap)
    {
        reverseMap(itr.first, itr.second);
    }
}


void IMDBData::reverseMap(const std::string& actorName, const std::vector<std::string>& movies)
{
	for(const auto& name : movies)
    {
        // if the movie is already in the keys
        if(mMoviesToActorsMap.find(name) != mMoviesToActorsMap.end())
        {
            // push back actor to its values
            mMoviesToActorsMap[name].push_back(actorName);
        }
        // if the movie is not in the keys
        else
        {
            mMoviesToActorsMap[name] = std::vector<std::string>();
            mMoviesToActorsMap[name].push_back(actorName);
        }
    }
}


const std::vector<std::string>& IMDBData::getMoviesFromActor(const std::string& actorName)
{
    auto itr = mActorsToMoviesMap.find(actorName);
	if( itr != mActorsToMoviesMap.end())
    {
        return itr->second;
    }
    return sEmptyVector;
}


const std::vector<std::string>& IMDBData::getActorsFromMovie(const std::string& movieName)
{
    auto itr = mMoviesToActorsMap.find(movieName);
    if( itr != mMoviesToActorsMap.end())
    {
        return itr->second;
    }
    return sEmptyVector;
}


void IMDBData::createGraph()
{
    // determine whether a actor pair has been connected before
    bool hasConnection = false;
	for(const auto& pair : mMoviesToActorsMap)
    {
        const std::string& movieName = pair.first;
        // connect each actors in the vector with the movie's name
        for(int i = 0; i < pair.second.size() - 1; i++)
        {
            ActorNode* fromActor = mGraph.getActorNode(pair.second[i]);
            // get the toActor: start from the next actor after the fromActor
            for(int j = i + 1; j < pair.second.size(); j++)
            {
                ActorNode* toActor = mGraph.getActorNode(pair.second[j]);
                // test if the two actors already has a connection
                for(const auto& edge : toActor->mEdges)
                {
                    if(edge.mOtherActor == fromActor)
                    {
                        hasConnection = true;
                    }
                }
                // connect the two actors if they haven't benn connected before
                if(!hasConnection)
                {
                    mGraph.createActorEdge(fromActor, toActor, movieName);
                }
                else
                {
                    hasConnection = false;
                }
            }
        }
        
    }
}


std::string IMDBData::findRelationship(const std::string& fromActor, const std::string& toActor)
{
   
    // a string stream to store the output result
    std::ostringstream result;
    // check if both actors are in the hash table
	if(!mGraph.containsActor(fromActor) && !mGraph.containsActor(toActor))
    {
        result << fromActor << " is unknown!\n" << toActor << " is unknown!\n";
        return result.str();
    }
    else if(!mGraph.containsActor(fromActor))
    {
        result << fromActor << " is unknown!\n";
        return result.str();
    }
    else if(!mGraph.containsActor(toActor))
    {
        result << toActor << " is unknown!\n";
        return result.str();
    }
    // initialize a queue for BFS
    std::queue<ActorNode*> actorQueue;
    // initialize a ptr to point at the current actor in search
    ActorNode* curr = nullptr;
    // enqueue the fromActor
    actorQueue.push(mGraph.getActorNode(fromActor));
    // loop until the queue is empty
    while(!actorQueue.empty())
    {
        // set curr to the top node in queue and dequeue
        curr = actorQueue.front();
        actorQueue.pop();
        // check if the node equals to our target
        if(curr == mGraph.getActorNode(toActor))
        {
            // build the first two lines of the return string
            result << "Found a path! (" << curr->mPath.size() << " hops)\n" << fromActor << " was in...\n";
            // loop until the last pair node in target actor's mPath
            for(int i = 0; i < curr->mPath.size()-1; i++)
            {
                result << curr->mPath[i].getMovieName() << " with " << curr->mPath[i].getActorName() << " who was in...\n";
            }
            // add the target actor itself
            result << curr->mPath[curr->mPath.size()-1].getMovieName() << " with " << curr->mPath[curr->mPath.size()-1].getActorName() << "\n";
            // clear the graph
            mGraph.clearVisited();
            // return result
            return result.str();
            
        }
        else if(!curr->mIsVisited)
        {
            // set isVisit to true to prevent loop back
            curr->mIsVisited = true;
            for(const auto node : curr->mEdges)
            {
                // if the connected actor had not been visited
                if(!node.mOtherActor->mIsVisited)
                {
                    // enqueue the other actor
                    actorQueue.push(node.mOtherActor);
                    // set the path for the other actor
                    if(node.mOtherActor->mPath.empty())
                    {
                        // set the other actor's path equals to the mother actor's path
                        node.mOtherActor->mPath = curr->mPath;
                        // push the other actor itself into its path
                        node.mOtherActor->mPath.push_back(PathPair(node.mMovieName, node.mOtherActor->mName));
                        
                    }
                }
            }
            
        }
        
    }
    // reset the graph
    mGraph.clearVisited();
    return "Didn't find a path.\n";
    
    
}


// Leave here! Do not edit!!!
// DO NOT REMOVE THIS LINE
std::vector<std::string> IMDBData::sEmptyVector;
