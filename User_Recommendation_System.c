#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_RECOMMENDATIONS 15

// Structure for linked list nodes (followers and followings)
//struct User;

struct FollowerNode {
    struct User* follower;
    struct FollowerNode* next;
};

struct FollowingNode {
    struct User* following;
    struct FollowingNode* next;
};

// Structure for user profiles
struct User {
    int userId;
    char name[50];
    struct FollowerNode* followers;  // Linked list of followers
    struct FollowingNode* followings; // Linked list of followings
    struct User* nextUser; // Pointer to the next user in the system
};

// Global head of the user list
struct User* userListHead = NULL;

// Function prototypes
struct User* createUser(int userId, char* name);
void followUser(struct User* user, struct User* toFollow);
void displayFollowers(struct User* user);
void displayFollowings(struct User* user);
int countMutualFollowers(struct User* user1, struct User* user2);
struct User* findUserById(int userId);
void createNewUser();
void calculateMutualFollowers();
void followUserAtRuntime();
void displayAllUsers();
void getRecommendations(struct User* user);
void initializeUsers();
void createRandomConnections();
void displayAllUsersWithoutFollowDetails();
void displayFollowing(struct User* user);
void unfollowUser(struct User* user, struct User* toUnfollow);
void removeFollower(struct User* user, struct User* follower);
void deleteUser(int userId);
// Utility functions
struct FollowerNode* createFollowerNode(struct User* follower);



int main() {
    srand(time(0)); // Seed for randomization

    // Initialize users and random interconnections
    initializeUsers();

    int choice;
    do {
        printf("\n<<Menu>>\n");
        printf("1. Create a user\n");
        printf("2. Follow a user\n");
        printf("3. Recommend a user\n");
        printf("4. Display followers of user\n");
        printf("5. Show mutual followers\n");
        printf("6. Show following list\n");
        printf("7. Display a user without followers and followings details\n");
        printf("8. Display all users with details\n");
        printf("9. Unfollow a user\n");
        printf("10. Remove a follower\n");
        printf("11. Delete a user\n");
        printf("12. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: 
                createNewUser();
                break;
            case 2:
                followUserAtRuntime();
                break;                
            case 3:
                {
                int userId;
                printf("Enter your user ID: ");
                scanf("%d", &userId);

                struct User* user = findUserById(userId);
                if (user) {
                    getRecommendations(user);
                } else {
                    printf("User not found.\n");
                }
                break;
            }
            case 4:
                {
                int userId;
                printf("Enter the user ID: ");
                scanf("%d", &userId);
                struct User* user = findUserById(userId);
                if (user) {
                    printf("\nFollowers of %s:\n", user->name);
                    displayFollowers(user);
                } else {
                    printf("User not found.\n");
                }
                break;
            }
            case 5:
                calculateMutualFollowers();
                break;

            case 6: {
                int userId;
                printf("Enter the user ID: ");
                scanf("%d", &userId);
                struct User* user = findUserById(userId);
                if (user) {
                    displayFollowing(user);
                } else {
                    printf("User not found.\n");
                }
                break;
            }
            case 7: 
                {
                displayAllUsersWithoutFollowDetails();
                break;
            }
            case 8: 
                displayAllUsers();
                break;

            case 9: {
                int userId, toUnfollowId;
                printf("Enter your user ID: ");
                scanf("%d", &userId);
                printf("Enter the ID of the user you want to unfollow: ");
                scanf("%d", &toUnfollowId);

                struct User* user = findUserById(userId);
                struct User* toUnfollow = findUserById(toUnfollowId);

                if (user && toUnfollow) {
                    unfollowUser(user, toUnfollow);
                    printf("%s has unfollowed %s.\n", user->name, toUnfollow->name);
                } else {
                    printf("One or both users not found.\n");
                }
                break;
            }
            case 10: {
                int userId, toRemoveId;
                printf("Enter the user ID: ");
                scanf("%d", &userId);
                printf("Enter the ID of the follower to remove: ");
                scanf("%d", &toRemoveId);

                struct User* user = findUserById(userId);
                struct User* toRemove = findUserById(toRemoveId);

                if (user && toRemove) {
                    removeFollower(user, toRemove);
                    printf("%s has removed %s as a follower.\n", user->name, toRemove->name);
                } else {
                    printf("One or both users not found.\n");
                }
                break;
            }
            case 11: {
                int userId;
                printf("Enter the user ID: ");
                scanf("%d", &userId);
                deleteUser(userId);
                break;
            }
            case 12:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 12);

    return 0;
}

// Function to create a user
struct User* createUser(int userId, char* name) {
    struct User* newUser = (struct User*)malloc(sizeof(struct User));
    newUser->userId = userId;
    strcpy(newUser->name, name);
    newUser->followers = NULL;
    newUser->followings = NULL;
    newUser->nextUser = NULL;

    // Add to the global user list
    if (!userListHead) {
        userListHead = newUser;
    } else {
        struct User* temp = userListHead;
        while (temp->nextUser) {
            temp = temp->nextUser;
        }
        temp->nextUser = newUser;
    }
    return newUser;
}

// Function to create a follower node
struct FollowerNode* createFollowerNode(struct User* follower) {
    struct FollowerNode* newNode = (struct FollowerNode*)malloc(sizeof(struct FollowerNode));
    newNode->follower = follower;
    newNode->next = NULL;
    return newNode;
}

// Function to follow a user
void followUser(struct User* user, struct User* toFollow) {
    if (!user || !toFollow) return;

    // Add to the following list of the current user
    struct FollowingNode* newFollowing = (struct FollowingNode*)malloc(sizeof(struct FollowingNode));
    newFollowing->following = toFollow;
    newFollowing->next = user->followings;
    user->followings = newFollowing;

    // Add to the followers list of the target user
    struct FollowerNode* newFollower = createFollowerNode(user);
    newFollower->next = toFollow->followers;
    toFollow->followers = newFollower;
}

// Function to display followers of a user
void displayFollowers(struct User* user) {
    struct FollowerNode* temp = user->followers;
    if (!temp) {
        printf("No followers.\n");
        return;
    }
    while (temp) {
        printf("- %s (ID: %d)\n", temp->follower->name, temp->follower->userId);
        temp = temp->next;
    }
}

// Function to display followings of a user
void displayFollowings(struct User* user) {
    struct FollowingNode* temp = user->followings;
    if (!temp) {
        printf("Not following anyone.\n");
        return;
    }
    while (temp) {
        printf("- %s (ID: %d)\n", temp->following->name, temp->following->userId);
        temp = temp->next;
    }
}

// Function to count mutual followers
int countMutualFollowers(struct User* user1, struct User* user2) {
    int count = 0;
    struct FollowerNode* temp1 = user1->followers;
    while (temp1) {
        struct FollowerNode* temp2 = user2->followers;
        while (temp2) {
            if (temp1->follower == temp2->follower) {
                count++;
                break;
            }
            temp2 = temp2->next;
        }
        temp1 = temp1->next;
    }
    return count;
}

// Function to find a user by ID
struct User* findUserById(int userId) {
    struct User* temp = userListHead;
    while (temp) {
        if (temp->userId == userId) return temp;
        temp = temp->nextUser;
    }
    return NULL;
}

// Function to create a new user at runtime
void createNewUser() {
    int userId;
    char name[50];
    printf("Enter user ID: ");
    scanf("%d", &userId);
    printf("Enter user name: ");
    scanf("%s", name);

    if (findUserById(userId)) {
        printf("User ID already exists.\n");
        return;
    }

    struct User* newUser = createUser(userId, name);
    printf("User %s (ID: %d) created successfully.\n", newUser->name, newUser->userId);
}

// Function to calculate mutual followers dynamically
void calculateMutualFollowers() {
    int userId1, userId2;
    printf("Enter the first user ID: ");
    scanf("%d", &userId1);
    printf("Enter the second user ID: ");
    scanf("%d", &userId2);

    struct User* user1 = findUserById(userId1);
    struct User* user2 = findUserById(userId2);

    if (user1 && user2) {
        int mutualCount = countMutualFollowers(user1, user2);
        printf("Mutual followers between %s and %s: %d\n", user1->name, user2->name, mutualCount);
    } else {
        printf("One or both users not found.\n");
    }
}

// Function to follow another user dynamically
void followUserAtRuntime() {
    int userId, toFollowId;
    printf("Enter your user ID: ");
    scanf("%d", &userId);
    printf("Enter the ID of the user you want to follow: ");
    scanf("%d", &toFollowId);

    struct User* user = findUserById(userId);
    struct User* toFollow = findUserById(toFollowId);

    if (user && toFollow) {
        followUser(user, toFollow);
        printf("%s is now following %s.\n", user->name, toFollow->name);
    } else {
        printf("One or both users not found.\n");
    }
}

// Function to display all users with details
void displayAllUsers() {
    struct User* temp = userListHead;
    if (!temp) {
        printf("No users exist.\n");
        return;
    }

    while (temp) {
        printf("\nUser ID: %d, Name: %s\n", temp->userId, temp->name);

        // Display followers
        printf("Followers:\n");
        struct FollowerNode* followerTemp = temp->followers;
        if (!followerTemp) {
            printf("- No followers.\n");
        } else {
            while (followerTemp) {
                printf("- %s (ID: %d)\n", followerTemp->follower->name, followerTemp->follower->userId);
                followerTemp = followerTemp->next;
            }
        }

        // Display followings
        printf("Following:\n");
        struct FollowingNode* followingTemp = temp->followings;
        if (!followingTemp) {
            printf("- Not following anyone.\n");
        } else {
            while (followingTemp) {
                printf("- %s (ID: %d)\n", followingTemp->following->name, followingTemp->following->userId);
                followingTemp = followingTemp->next;
            }
        }

        temp = temp->nextUser; // Move to the next user
    }
}

// Function to initialize users
void initializeUsers() {
    // Indian user names and IDs
    char* userNames[] = {
        "Aarav", "Vivaan", "Aditya", "Vihaan", "Arjun", "Sai", "Reyansh", "Krishna", "Ishaan", "Shaurya",
        "Ayaan", "Aryan", "Kartik", "Krish", "Rohan", "Ansh", "Dhruv", "Yash", "Zain", "Laksh",
        "Saanvi", "Ananya", "Aadhya", "Diya", "Pooja", "Madhavi", "Meera", "Tanya", "Simran", "Riya",
        "Sanya", "Nandini", "Priya", "Radhika", "Aarti", "Sonali", "Sneha", "Neha", "Rupal", "Komal",
        "Rekha", "Shreya", "Shalini", "Swara", "Rupal", "Kavya", "Surbhi", "Anjali", "Neelam", "Madhuri"
    };

    for (int i = 0; i < 50; i++) {
        createUser(i + 1, userNames[i]);
    }

    // Create random followings between users
    createRandomConnections();
}

// Function to create random connections between users
void createRandomConnections() {
    struct User* temp1 = userListHead;
    while (temp1) {
        struct User* temp2 = userListHead;
        while (temp2) {
            if (temp1 != temp2) {
                // Randomly decide whether to create a following relationship
                if (rand() % 2 == 0) {
                    followUser(temp1, temp2);
                }
            }
            temp2 = temp2->nextUser;
        }
        temp1 = temp1->nextUser;
    }
}

// Function to display all users in the system without followers and followings details
void displayAllUsersWithoutFollowDetails() {
    struct User* temp = userListHead;
    if (!temp) {
        printf("No users exist.\n");
        return;
    }

    while (temp) {
        printf("\nUser ID: %d, Name: %s\n", temp->userId, temp->name);
        temp = temp->nextUser; // Move to the next user
    }
}

// Function to display the following list of a user
void displayFollowing(struct User* user) {
    if (!user || !user->followings) {
        printf("%s is not following anyone.\n", user->name);
        return;
    }

    struct FollowingNode* temp = user->followings;
    printf("Following list of %s (ID: %d):\n", user->name, user->userId);
    while (temp) {
        printf("- %s (ID: %d)\n", temp->following->name, temp->following->userId);
        temp = temp->next;
    }
}

// Function to unfollow a user
void unfollowUser(struct User* user, struct User* toUnfollow) {
    if (!user || !toUnfollow) {
        printf("User or user to unfollow not found.\n");
        return;
    }

    // Remove from the current user's following list
    struct FollowingNode* prevFollowing = NULL, *tempFollowing = user->followings;
    while (tempFollowing) {
        if (tempFollowing->following == toUnfollow) {
            if (prevFollowing) {
                prevFollowing->next = tempFollowing->next;
            } else {
                user->followings = tempFollowing->next; // Removing the first node
            }
            free(tempFollowing);
            printf("%s has unfollowed %s.\n", user->name, toUnfollow->name);
            break;
        }
        prevFollowing = tempFollowing;
        tempFollowing = tempFollowing->next;
    }

    // Remove from the target user's followers list
    struct FollowerNode* prevFollower = NULL, *tempFollower = toUnfollow->followers;
    while (tempFollower) {
        if (tempFollower->follower == user) {
            if (prevFollower) {
                prevFollower->next = tempFollower->next;
            } else {
                toUnfollow->followers = tempFollower->next; // Removing the first node
            }
            free(tempFollower);
            break;
        }
        prevFollower = tempFollower;
        tempFollower = tempFollower->next;
    }
}

// Function to remove a follower
void removeFollower(struct User* user, struct User* follower) {
    if (!user || !follower) {
        printf("User or follower not found.\n");
        return;
    }

    // Remove the follower from the user’s followers list
    struct FollowerNode* prev = NULL, *temp = user->followers;
    while (temp) {
        if (temp->follower == follower) {
            if (prev) {
                prev->next = temp->next;
            } else {
                user->followers = temp->next; // Removing the first node
            }
            free(temp);
            printf("%s is no longer a follower of %s.\n", follower->name, user->name);
            return;
        }
        prev = temp;
        temp = temp->next;
    }

    printf("%s is not a follower of %s.\n", follower->name, user->name);
}

// Function to delete a user and their associated data
void deleteUser(int userId) {
    struct User* user = findUserById(userId);
    if (!user) {
        printf("User with ID %d not found.\n", userId);
        return;
    }

    // Remove the user from the followers list of users who follow them
    struct FollowerNode* followerTemp = user->followers;
    while (followerTemp) {
        struct FollowerNode* toRemove = followerTemp;
        followerTemp = followerTemp->next;

        // Remove the user from the follower's following list
        struct FollowingNode* followingTemp = toRemove->follower->followings;
        struct FollowingNode* prevFollowing = NULL;
        while (followingTemp) {
            if (followingTemp->following == user) {
                if (prevFollowing) {
                    prevFollowing->next = followingTemp->next;
                } else {
                    toRemove->follower->followings = followingTemp->next;
                }
                free(followingTemp);
                break;
            }
            prevFollowing = followingTemp;
            followingTemp = followingTemp->next;
        }
        free(toRemove);
    }

    // Remove the user from the following list of users they are following
    struct FollowingNode* followingTemp = user->followings;
    while (followingTemp) {
        struct FollowingNode* toRemove = followingTemp;
        followingTemp = followingTemp->next;

        // Remove the user from the followed user's followers list
        struct FollowerNode* followerTemp = toRemove->following->followers;
        struct FollowerNode* prevFollower = NULL;
        while (followerTemp) {
            if (followerTemp->follower == user) {
                if (prevFollower) {
                    prevFollower->next = followerTemp->next;
                } else {
                    toRemove->following->followers = followerTemp->next;
                }
                free(followerTemp);
                break;
            }
            prevFollower = followerTemp;
            followerTemp = followerTemp->next;
        }
        free(toRemove);
    }

    // Now, remove the user from the global user list
    struct User* temp = userListHead;
    struct User* prevUser = NULL;
    while (temp) {
        if (temp == user) {
            if (prevUser) {
                prevUser->nextUser = temp->nextUser;
            } else {
                userListHead = temp->nextUser; // Head is being deleted
            }
            free(temp);
            printf("User with ID %d and their data has been deleted.\n", userId);
            return;
        }
        prevUser = temp;
        temp = temp->nextUser;
    }
}



// Function to get a random number between min and max (inclusive)
int getRandomNumber(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Function to shuffle an array (Fisher-Yates shuffle)
void shuffleArray(struct User* array[], int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = getRandomNumber(0, i);
        struct User* temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

// Function to count the number of followings for a given user
int countFollowings(struct User* user) {
    int count = 0;
    struct FollowingNode* temp = user->followings;
    while (temp) {
        count++;
        temp = temp->next;
    }
    return count;
}

// Function to select N random followings from a user's following list
void selectRandomFollowings(struct User* user, struct User* selected[], int n) {
    struct User* allFollowings[100];
    int count = 0;

    struct FollowingNode* temp = user->followings;
    while (temp && count < 100) {
        allFollowings[count++] = temp->following;
        temp = temp->next;
    }

    shuffleArray(allFollowings, count);

    for (int i = 0; i < n && i < count; i++) {
        selected[i] = allFollowings[i];
    }
}

// Function to select N random followers from a user's follower list
void selectRandomFollowers(struct User* user, struct User* selected[], int n) {
    struct User* allFollowers[100]; 
    int count = 0;

    struct FollowerNode* temp = user->followers;
    while (temp && count < 100) {
        allFollowers[count++] = temp->follower;
        temp = temp->next;
    }

    shuffleArray(allFollowers, count);

    for (int i = 0; i < n && i < count; i++) {
        selected[i] = allFollowers[i];
    }
}

// Function to get friend recommendations for a user
void getRecommendations(struct User* user) {
    if (!user) return;

    int numFollowings = countFollowings(user);
    if (numFollowings == 0) {
        printf("No recommendations available for %s (ID: %d).\n", user->name, user->userId);
        return;
    }

    // Determine number of followings to consider (maximum 5)
    int followingsToConsider = (numFollowings > 5) ? 5 : numFollowings;

    struct User* selectedFollowings[5];
    selectRandomFollowings(user, selectedFollowings, followingsToConsider);

    struct User* recommendations[MAX_RECOMMENDATIONS];
    int recommendationCount = 0;

    int recommendedUserIds[100] = {0};
    recommendedUserIds[user->userId] = 1;

    // Iterate over selected followings and get their followers
    for (int i = 0; i < followingsToConsider; i++) {
        struct User* followingUser = selectedFollowings[i];
        struct User* followers[3];
        selectRandomFollowers(followingUser, followers, 3);

        for (int j = 0; j < 3; j++) {
            struct User* potentialRecommendation = followers[j];

            // If the user is not already recommended, add them
            if (potentialRecommendation && !recommendedUserIds[potentialRecommendation->userId]) {
                recommendations[recommendationCount++] = potentialRecommendation;
                recommendedUserIds[potentialRecommendation->userId] = 1;

                // If we've reached the max recommendations, stop
                if (recommendationCount == MAX_RECOMMENDATIONS) {
                    break;
                }
            }
        }

        // Stop if we reach the max recommendations
        if (recommendationCount == MAX_RECOMMENDATIONS) {
            break;
        }
    }

    if (recommendationCount == 0) {
        printf("No recommendations available for %s (ID: %d).\n", user->name, user->userId);
    } else {
        printf("\nFriend Recommendations for %s (ID: %d):\n", user->name, user->userId);
        for (int i = 0; i < recommendationCount; i++) {
            printf("- %s (ID: %d)\n", recommendations[i]->name, recommendations[i]->userId);
        }
    }
}
