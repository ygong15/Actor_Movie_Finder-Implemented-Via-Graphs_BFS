//  Created by Sanjay Madhav on 1/1/2020.
//  Copyright © 2020 Sanjay Madhav. All rights reserved.

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!! DO NOT EDIT THIS FILE !!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include "IMDBGraph.h"
#include <iostream>

// Function: containsActor
// Purpose: Checks whether the requested actor is in the graph
// Input: Name of actor (by const reference)
// Returns: True if the actor is in the graph
bool IMDBGraph::containsActor(const std::string& actorName)
{
    return (mActorNodeMap.find(actorName) != mActorNodeMap.end());
}

// Function: getActorNode
// Purpose: If the actor doesn't exist, creates a node for the actor.
// Otherwise, returns the ActorNode associated w/ the specified actor.
// Input: Name of actor (by const reference)
// Returns: Pointer to ActorNode associated with the actor
ActorNode* IMDBGraph::getActorNode(const std::string& actorName)
{
    // If the actor already exists, return the pointer
    // to its ActorNode
    if (mActorNodeMap.find(actorName) != mActorNodeMap.end())
    {
        return mActorNodeMap[actorName];
    }
    else
    {
        // Otherwise, make a new ActorNode for this actor
        ActorNode* newNode = new ActorNode(actorName);
		mActorNodeMap.insert(std::make_pair(actorName, newNode));
        return newNode;
    }
}

// Function: createActorEdge
// Purpose: Creates a directed edge from firstActor to secondActor, and
// from secondActor to firstActor. Both edges have the specified movie name as
// their label.
// Input: Pointer to the first ActorNode, pointer to the second ActorNode,
// and the movie name (by reference)
// Returns: Nothing
void IMDBGraph::createActorEdge(ActorNode* firstActor, ActorNode* secondActor, const std::string& movieName)
{
    if (firstActor == nullptr)
    {
        std::cerr << "createActorEdge: first actor is null!" << std::endl;
        exit(1);
    }
    
    if (secondActor == nullptr)
    {
        std::cerr << "createActorEdge: second actor is null!" << std::endl;
        exit(1);
    }
    
    // Make an edge from the first actor to the second actor
    firstActor->mEdges.emplace_back(movieName, secondActor);
    
    // Make an edge from the second actor to the first actor
    secondActor->mEdges.emplace_back(movieName, firstActor);
}

// Helper function used by "clearVisited"
void clearVisitedHelper(const std::string& key, ActorNode* value)
{
    value->mIsVisited = false;
    value->mPath.clear();
}

// Function: clearVisited
// Purpose: Clears the mIsVisited flag on every ActorNode in the graph,
// and also clears out the PathPair lists.
// Input: None
// Returns: Nothing
void IMDBGraph::clearVisited()
{
    for (auto p : mActorNodeMap)
    {
        clearVisitedHelper(p.first, p.second);
    }
}

// Helper function used by destructor
void deleteHelper(const std::string& key, ActorNode* value)
{
    delete value;
}

// Function: Destructor
// Purpose: Deletes all of the actor nodes in the graph
// Input: None
// Returns: Nothing
IMDBGraph::~IMDBGraph()
{
    for (auto p : mActorNodeMap)
    {
        deleteHelper(p.first, p.second);
    }
}

// ActorNode destructor
ActorNode::~ActorNode()
{

}
