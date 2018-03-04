#include <fstream>
#include <iostream>
#include <string>
#include "SongLibrary.h"

//////////////////DO NOT CHANGE THIS CODE////////////////////////

//Construct a Song object with values
Song::Song(const std::string& title, const std::string& artist, 
     const std::string& album, unsigned int year){
	this->title = title;
	this->artist = artist;
	this->album = album;
	this->year = year;
	used = false;
	this->song_group_ptr = NULL;
}

//Song equality doesn't depend on pointers or being used
bool operator== (const Song& s1, const Song& s2){
	return (s1.getYear() == s2.getYear()) && (s1.getTitle() == s2.getTitle())
          && (s1.getAlbum() == s2.getAlbum()) && (s1.getArtist() == s2.getArtist());
}

//Modifier for group pointer
void Song::updateSongGroupPointer(SongGroupNode* ptr){
	if(ptr){
		//Check that ptr is actually for this Song.
		assert(ptr->m_song->value == *this); 
		song_group_ptr = ptr;
	}
	else{
		song_group_ptr = NULL;
	}
}

//Make output easier by overload operator<<
std::ostream& operator<< (std::ostream& ostr, const Song& song){
	if(!song.getSongGroupPointer()){
		ostr << "\"" << song.getTitle() << "\" by " << song.getArtist() << " on \"" 
	       << song.getAlbum() << "\" (" << song.getYear() << ")" << std::endl;
	}
	else{
		ostr << "\"" << song.getTitle() << "\" by " << song.getArtist() << " on \"" 
		<< song.getAlbum() << "\" (" << song.getYear() << ")" << " in Song Group "
		<< song.getSongGroupPointer()->id << std::endl;
	}
	return ostr;
}

/////////////////////////YOUR CODE STARTS HERE/////////////////////////////////


//////////////////////////////GROUP FUNCTIONS/////////////////////////////////

/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
-id is the ID of the list we are searching for
BEHAVIOR:
None
RETURN:
NULL if no list matches the ID, otherwise returns the GroupListNode* containing
the head of the list with ID id.
*/
GroupListNode* GroupExists(GroupListNode* song_groups, GroupID id) {
	for (GroupListNode* temp = song_groups; temp != NULL;temp = temp->ptr) {
		if (temp->value->id == id) return temp;
	}
	return NULL;
}

/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
-id is the ID of the list we are creating
BEHAVIOR:
Add a new song group to song_groups, give the new song group ID id. Order of
song groups is not guaranteed.
RETURN:
None
*/
void MakeGroup(GroupListNode*& song_groups, GroupID id){
	SongGroupNode* newgroup = new SongGroupNode;// create a new heap for a new group
	newgroup->id = id;// initial the newgroup with requested conditions
	newgroup->next_by_artist_ptr = NULL;
	newgroup->next_song_ptr = NULL;
	newgroup->prev_by_year_ptr = NULL;
	newgroup->prev_song_ptr = NULL;
	newgroup->m_song = NULL;
	if (song_groups == NULL) { 
		// if there's nothing in the original list, then simply link the node to the newgroup
		song_groups = new GroupListNode;
		song_groups->ptr = NULL;
		song_groups->value = newgroup;
	}
	else {
		//if there's something previously, go to the end of this original list,
		//then link the last one with newgroup
		GroupListNode* temp;
		for (temp = song_groups; temp->ptr != NULL; temp = temp->ptr);
		GroupListNode* newnode = new GroupListNode;
		newnode->ptr = NULL;
		newnode->value = newgroup;
		temp->ptr = newnode;
	}
}


/*
ARGUMENTS:
-group_head points to the head of a song group
-song is the Song we are checking the group for
BEHAVIOR:
None
RETURN:
If song is found in the group return true. If song is not found,
return false.
*/
bool SongInGroup(SongGroupNode* group_head, const Song& song){
	SongGroupNode* temp = group_head;
	//go through the song list to find the song
	for (; temp != NULL; temp = temp->next_song_ptr) {
		if (temp->m_song->value.getTitle() == song.getTitle()) return 1;
	}
	return 0;
}

/*
ARGUMENTS:
-library_ptr points to the LibraryNode* containing the Song we want to add.
-list_ptr is the head of the song group we want to add the song to.
BEHAVIOR:
Modify the group pointed to by list_ptr to reflect that the song is in the group.
This may change the length of the list. Modifies the Song's members as well.
RETURN:
None
*/
void AddToGroup(LibraryNode* library_ptr, GroupListNode* list_ptr){
	if (list_ptr->value->m_song == NULL) {
		// if there's nothing initially, simply link my_song to the library node
		list_ptr->value->m_song = library_ptr;
		library_ptr->value.updateSongGroupPointer(list_ptr->value);
	}
	else {
		// or there's something:
		SongGroupNode* artist = NULL;
		SongGroupNode* temp = list_ptr->value;
		SongGroupNode* tail;//used to find the previous node
		for (; temp != NULL; temp = temp->next_song_ptr) {
		//go through to the end of the list
			if (temp->m_song->value.getArtist() == library_ptr->value.getArtist()) {
		//by the way, find the latest same artist		
				artist = temp;
			}
			if (temp->next_song_ptr == NULL) tail = temp;
		}
		//now we've found the correct position and propriate info to link a new song
		SongGroupNode* newsong = new SongGroupNode;
		newsong->id = tail->id;
		newsong->next_song_ptr = NULL;
		newsong->m_song = library_ptr;
		//if we've found a same artist then link that song's next_by_artist_ptr with this newsong
		if (artist!= NULL) artist->next_by_artist_ptr = newsong;
		//and assign this newsong's next_by_artist_ptr to NULL
		newsong->next_by_artist_ptr = NULL;
		// use tail to link a prev_song_ptr
		newsong->prev_song_ptr = tail;
		tail->next_song_ptr = newsong;
		//dont forget to update the group info for this new song
		library_ptr->value.updateSongGroupPointer(tail->next_song_ptr);//!!!!!!!!!!!!!!!
		//we've made some modification to the list, so recompute the prev_by_year_ptr is necesary
		RecomputePrevByYear(list_ptr);
	}
	//also remember to set the usage of song in library
	library_ptr->value.setUsedInGroup(1);//!!!!!!!!!!!!!!
}

/*
ARGUMENTS:
-library_ptr points to the LibraryNode* containing the Song we want to remove from a group.
-song_groups points to a singly-linked list of song group heads
BEHAVIOR:
Modifies the Song's member variables to reflect that it is not in a group. Modifies the group
so that the Song is not in the group. This function may cause song_groups to be modified as well.
RETURN:
None
*/
void RemoveFromGroup(LibraryNode* library_ptr, GroupListNode* song_groups) {
	//we should first find the right group to find the song(by id)
	GroupListNode* sg = song_groups;
	for (; sg!= NULL; sg = sg->ptr) {
		if (sg->value->id == library_ptr->value.getSongGroupPointer()->id) { 
			break;
		}
	}
	//because we will remove this song, so we should set the usage to 0
	//and clear the group pointer
	library_ptr->value.setUsedInGroup(0);
	library_ptr->value.updateSongGroupPointer(NULL);
	SongGroupNode* temp = sg->value;
	// in the process of removing, should also take care of the artists' nodes
	SongGroupNode* artist = NULL;
	for (; temp->next_song_ptr!=NULL;temp = temp->next_song_ptr) {
		if (temp->m_song->value.getTitle() == library_ptr->value.getTitle()) break;
		if (temp->m_song->value.getArtist() == library_ptr->value.getArtist()) {
			artist = temp;
		}
	}
	if (temp->prev_song_ptr == NULL) {
	//if this song to remove is the first song in the list:
		if (temp->next_song_ptr != NULL) {
		// but it's not the last song, so we need to set 
		//next song's previous pointer to NULL and recompute the year
			temp->next_song_ptr->prev_song_ptr = NULL;
			RecomputePrevByYear(sg);
			sg->value = temp->next_song_ptr;
			delete temp;
		}
		else { 
		// also it's the last song, which means it's the only song in the list
		// then we just need to set all the pointers inside to NULL
			temp->m_song = NULL; 
			temp->next_by_artist_ptr = NULL;
			temp->next_song_ptr = NULL;
			temp->prev_by_year_ptr = NULL;
			temp->prev_song_ptr = NULL;
		}
	}
	else {
	// it's not the first song in the list
		if (temp->next_song_ptr == NULL) {
			// but it's the last one
			//simply set previous song's next pointer to NULL
			// and set previous by artist song's next by artist pointer to NULL, if it has one
			temp->prev_song_ptr->next_song_ptr = NULL;
			if(artist!=NULL) artist->next_by_artist_ptr = NULL;
			delete temp;
		}
		else {
			// right in the middle of the group
			// same thing as above
			// recompute the year additionly
			temp->next_song_ptr->prev_song_ptr = temp->prev_song_ptr;
			temp->prev_song_ptr->next_song_ptr = temp->next_song_ptr;
			if(artist!=NULL) artist->next_by_artist_ptr = temp->next_by_artist_ptr;
			RecomputePrevByYear(sg);
		}
	}
}


/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
-list_ptr1 contains to the song group head of Group A
-list_ptr2 contains the song group head of Group B
BEHAVIOR:
Modify Group A so that at the end of its songs, the songs of Group B are added in the same
order they were found in Group B. Remove the now-empty Group B.
RETURN:
None
*/
void CombineGroups(GroupListNode*& song_groups, GroupListNode*& list_ptr1, GroupListNode*& list_ptr2){
	SongGroupNode* temp1 = list_ptr1->value;
	SongGroupNode* temp2 = list_ptr2->value;
	SongGroupNode* artist = NULL;
	if (temp2->m_song != NULL) {
	// if group 2 has something
		for (; temp2 != NULL; temp2 = temp2->next_song_ptr) {
			// go through the group to change id
			temp2->id = list_ptr1->value->id;
			// we should first alter the artists nodes as if the list has been already combined
			for (; temp1 != NULL; temp1 = temp1->next_song_ptr) {
				if (temp1->m_song->value.getArtist() == temp2->m_song->value.getArtist()) {
					artist = temp1;
				}
			}
			if (artist != NULL && artist->next_by_artist_ptr == NULL) {
				artist->next_by_artist_ptr = temp2;
			}
		}
		temp2 = list_ptr2->value;
		temp1 = list_ptr1->value;
		if (temp1->m_song == NULL) {
			//if there's nothing in group 1
			delete temp1;
			//easily delete the empty head and relink it to group2
			list_ptr1->value = temp2;
		}
		else {
			// if there are both someting in two groups
			for (; temp1->next_song_ptr != NULL; temp1 = temp1->next_song_ptr);
			// go to the end of group 1
			temp1->next_song_ptr = temp2;
			temp2->prev_song_ptr = temp1;
			//link it with the head of group2
			RecomputePrevByYear(list_ptr1);
			// and recompute the year
		}
	}
	// now we just need to get over with group 2's empty head
	GroupListNode* list = song_groups;
	if (list == list_ptr2) { delete list_ptr2; }
	//if group2 is first group in the group list,we just  delete it
	else {
		//if it's not, we should also relink the nodes
		for (; list->ptr != list_ptr2; list = list->ptr);
		list->ptr = list_ptr2->ptr;
		delete list_ptr2;
	}
}

/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
-list_ptr is the head of the song group we want to remove.
BEHAVIOR:
Remove the group from the list of song groups. No Song objects should remain associated with the
group, and the function should modify Song member variables appropriately to reflect this. Any
memory associated with the group must be released.
RETURN:
None*/

void RemoveGroup(GroupListNode*& song_groups, GroupListNode* list_ptr) {
	if (list_ptr->value->next_song_ptr != NULL) {
		// if there's something in the group, then we need to delete them first
		SongGroupNode* temp = list_ptr->value;
		SongGroupNode* next;
		for (;temp != NULL; temp = next) {
			next = temp->next_song_ptr;
			delete temp;
		}
	}
	//once cleared up the group, now we just remove the group head from the group head lists
	GroupListNode* last = song_groups;
	if (last == list_ptr) {
		//  if the group to be removed is the fisrt group in the list
		song_groups = list_ptr->ptr;
		delete list_ptr;
	}
	else {
		for (; last->ptr != list_ptr; last = last->ptr);
		last->ptr = list_ptr->ptr;
		delete list_ptr;
	}
}

/*
ARGUMENTS:
-list_ptr is the head of the song group we want to add the song to.
BEHAVIOR:
Recompute all prev_by_year_ptr members in the song group pointed to by
list_ptr.
RETURN:
None
*/
void RecomputePrevByYear(GroupListNode* list_ptr){
	SongGroupNode* temp = list_ptr->value;
	// go through the song list
	for (; temp != NULL; temp = temp->next_song_ptr) {
		// for each song, we search for the closest smaller year before it
			SongGroupNode* temp_song = list_ptr->value;
			SongGroupNode* preyear = NULL;
			for (; temp_song != temp && temp_song != NULL; temp_song = temp_song->next_song_ptr) {
				if (temp_song->m_song->value.getYear()
					< temp->m_song->value.getYear()) {
					preyear = temp_song;
				}
			}
			temp->prev_by_year_ptr = preyear;
	}
}

////////////////////////////PRINT FUNCTIONS////////////////////////////////////

/*
ARGUMENTS:
-library_head points to the first LibraryNode* of the song library
-print_file points to an ostream to print output to
BEHAVIOR:
Print each song in the library in the order it was added.
RETURN:
None
*/
void PrintLibrary(LibraryNode* library_head,std::ostream& print_file){
	int count = 0;
	for (LibraryNode* temp = library_head; temp != NULL; temp = temp->ptr) {
		print_file << temp->value;
		count++;
	}
	print_file << "Library contains " << count << " song(s)\n";
}

/*
ARGUMENTS:
-group_head points to the first SongGroupNode* of the group to print
-print_file points to an ostream to print output to
BEHAVIOR:
Print each song in the group in the order it was added.
RETURN:
None
*/
void PrintGroupSequential(SongGroupNode* group_head,std::ostream& print_file){
	int count = 0;
	print_file << "List ID " << group_head->id << " contains:\n";
	if (group_head->m_song != NULL) {
		//if the first one is not the empty head
		for (SongGroupNode* temp = group_head; temp != NULL; temp = temp->next_song_ptr) {
			print_file << temp->m_song->value;
			count++;
		}
	}
	print_file << count << " song(s)\n";
	
}

/*
ARGUMENTS:
-group_ptr points to the location in a particular song group to start printing from
-print_file points to an ostream to print output to
BEHAVIOR:
Print a rewind list by following the prev_by_year_ptr pointers, starting from the song
pointed to by group_ptr.
RETURN:
None
*/
void PrintGroupRewind(SongGroupNode* group_ptr,std::ostream& print_file){
	print_file << "Rewind list for " << group_ptr->m_song->value.getYear() << "\n";
	SongGroupNode* temp = group_ptr;
	int count = 0;
	for (; temp != NULL; temp = temp->prev_by_year_ptr) {
		count++;
		print_file << "\t#" << count << ": " << temp->m_song->value;
	}
	print_file << "Rewind list contained " << count << " song(s).\n";
}

/*
ARGUMENTS:
-group_ptr points to the location in a particular song group to start printing from
-print_file points to an ostream to print output to
BEHAVIOR:
Print a rewind list by following the next_by_artist_ptr pointers, starting from the song
pointed to by group_ptr.
RETURN:
None
*/
void PrintGroupMarathon(SongGroupNode* group_ptr,std::ostream& print_file){
	
	SongGroupNode* temp = group_ptr;
	int count = 0;

	print_file << "Marathon list for " << temp->m_song->value.getArtist() << "\n";
	for (; temp != NULL; temp = temp->next_by_artist_ptr) {
		count++;
		print_file<< "\t#" << count << ": " << temp->m_song->value;
	}
	print_file << "Marathon list contained " << count << " song(s).\n";
	
}

/*
ARGUMENTS:
-group_head points to the first SongGroupNode* of the group to analyze
-print_file points to an ostream to print output to
BEHAVIOR:
Find the rewind list with the most songs in the song group pointed to by group_head.
Print the rewind list and the song count to print_file.
RETURN:
None
*/
void PrintLongestGroupRewind(GroupListNode* group_head,std::ostream& print_file){
	SongGroupNode* temp = group_head->value->next_song_ptr;
	SongGroupNode* longest = NULL;
	int last_count = 0;
	//variable longest and last_count is used to store the song with largest rewind numbers and 
	// the largest rewind number
	if (temp->m_song != NULL) {
		for (; temp!= NULL; temp = temp->next_song_ptr) {
			SongGroupNode* temp_song = temp;
			int count = 0;
			for (; temp_song != NULL; temp_song = temp_song->prev_by_year_ptr) {
				count++;
			}
			if (count > last_count) {
				//if count of rewind is larger than last_count then we make substitutions
				last_count = count;
				longest = temp;
			}
		}
	}
	print_file << "Maximum size rewind list is for year " 
		<< longest->m_song->value.getYear() << "\n";
	temp = longest;
	int count = 0;
	for (; temp != NULL; temp = temp->prev_by_year_ptr) {
		count++;
		print_file << "\t#" << count << ": " << temp->m_song->value;
	}
	print_file << "Longest rewind list contained " << count << " song(s).\n";
}

///////////////////////////////CLEANUP/////////////////////////////////////////

/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
BEHAVIOR:
Delete every group. Do not leak. Modifies all groups and the song_groups list.
Update every Song to reflect not being in a group.
RETURN:
None

*/
void DeleteAllSongGroups(GroupListNode*& song_groups){
	GroupListNode* group = song_groups;
	if (group != NULL) {
		GroupListNode* next_group;
		//next_group also with the following next_song, is used as
		// a step of the for loop, because if we have already deleted a song,
		// we can no more call its next pointer
		for (; group != NULL; group = next_group) {
			next_group = group->ptr;
			SongGroupNode* song = group->value;
			SongGroupNode* next_song = song->next_song_ptr;
			for (; song != NULL; song = next_song) {
				next_song = song->next_song_ptr;
				delete song;
			}
			delete group;
		}
	}
}
