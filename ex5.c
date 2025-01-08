#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Song
{
    char *title;
    char *artist;
    int year;
    char *lyrics;
    int streams;
} Song;

typedef struct Playlist
{
    char *name;
    Song **songs;
    int songsNum;
} Playlist;

void addSong(Playlist *playlist);
void showPlaylist(Playlist *playlist);

char *getStringInput()
{
    char temp[1024];
    scanf(" %[^\n]s", temp);
    char *str = (char *)malloc(strlen(temp) + 1);
    if (!str)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    strcpy(str, temp);
    return str;
}

void sortPlaylist(Playlist *playlist, int sortChoice)
{
    int songsNum = playlist->songsNum;

    for (int i = 0; i < songsNum - 1; i++)
    {
        for (int j = 0; j < songsNum - i - 1; j++)
        {
            switch (sortChoice)
            {
                // sort by year
            case 1:
                if (playlist->songs[j]->year > playlist->songs[j + 1]->year)
                {
                    Song *temp = playlist->songs[j];
                    playlist->songs[j] = playlist->songs[j + 1];
                    playlist->songs[j + 1] = temp;
                }
                break;
                // sort by streams ascending order
            case 2:
                if (playlist->songs[j]->streams > playlist->songs[j + 1]->streams)
                {
                    Song *temp = playlist->songs[j];
                    playlist->songs[j] = playlist->songs[j + 1];
                    playlist->songs[j + 1] = temp;
                }
                break;
                // sort by streams descending order
            case 3:
                if (playlist->songs[j]->streams < playlist->songs[j + 1]->streams)
                {
                    Song *temp = playlist->songs[j];
                    playlist->songs[j] = playlist->songs[j + 1];
                    playlist->songs[j + 1] = temp;
                }
                break;
                // sort alphabetically
            case 4:
                if (strcmp(playlist->songs[j]->title, playlist->songs[j + 1]->title) > 0)
                {
                    Song *temp = playlist->songs[j];
                    playlist->songs[j] = playlist->songs[j + 1];
                    playlist->songs[j + 1] = temp;
                }
                break;
            }
        }
    }
    printf("sorted\n");
}

void watchPlaylists(Playlist *playlists, int playlistCount)
{
    int playlistsChoice;
    if (playlistCount > 0)
    {
        for (int i = 0; i < playlistCount; i++)
        {
            if (playlists[i].name)
                printf("%d. %s\n", i + 1, playlists[i].name);
            if (i == playlistCount - 1)
            {
                printf("%d. Back to main menu\n", playlistCount + 1);
            }
        }
    }
    else
    {
        printf("1. Back to main menu\n");
    }

    scanf("%d", &playlistsChoice);
    if (playlistsChoice == playlistCount + 1)
    {
        return;
    }
    else if (playlistsChoice > 0 && playlistsChoice <= playlistCount)
    {
        printf("playlist %s:\n", playlists[playlistsChoice - 1].name);
        int actionsChoice;
        printf("\t1. Show Playlist\n\t2. Add Song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. Exit\n");
        scanf("%d", &actionsChoice);
        while (actionsChoice != 6)
        {
            switch (actionsChoice)
            {
            case 1:
                showPlaylist(&playlists[playlistsChoice - 1]);
                break;
            case 2:
                addSong(&playlists[playlistsChoice - 1]);
                break;
            case 3:
                deleteSong(&playlists[playlistsChoice - 1]);
                break;
            case 4:
                printf("choose:\n");
                printf("\t1. Sort by year\n\t2. Sort by steams - ascending order\n\t3. Sort by streams - descending order\n\t4. Sort alphabetically\n");
                int sortChoice;
                scanf("%d", &sortChoice);
                sortPlaylist(&playlists[playlistsChoice - 1], sortChoice);
                break;
            case 5:
                // play
                break;
            case 6:
                // exit
                break;
            }
        }
    }
}

void freeSong(Song *song)
{
    free(song->title);
    free(song->artist);
    free(song->lyrics);
    free(song);
}

void freePlaylist(Playlist *playlist)
{
    if (playlist)
    {
        if (playlist->songs)
        {
            for (int i = 0; i < playlist->songsNum; i++)
            {
                freeSong(playlist->songs[i]);
            }
            free(playlist->songs);
        }
        free(playlist->name);
        free(playlist);
    }
}

void playSong(Song *song)
{
    printf("Now playing %s:\n", song->title);
    printf("♪ %s ♪\n", song->lyrics);
    song->streams++;
}

void showPlaylist(Playlist *playlist)
{
    for (int i = 0; i < playlist->songsNum; i++)
    {
        printf("%d.\tTitle: %s\n", i + 1, playlist->songs[i]->title);
        printf("\tArtist: %s\n", playlist->songs[i]->artist);
        printf("\tReleased: %d\n", playlist->songs[i]->year);
        printf("\tStreams: %d\n", playlist->songs[i]->streams);
    }

    printf("choose a song to play, or 0 to quit:\n");
    int choice;
    scanf("%d", &choice);
    while (choice != 0)
    {
        if (choice > 0 && choice <= playlist->songsNum)
        {
            playSong(playlist->songs[choice - 1]);
        }
        else
        {
            printf("Invalid choice, please try again.\n");
        }
        printf("choose a song to play, or 0 to quit:\n");
        scanf("%d", &choice);
    }
    return;
}

void deleteSong(Playlist *playlist)
{
    for (int i = 0; i < playlist->songsNum; i++)
    {
        printf("%d.\tTitle: %s\n", i + 1, playlist->songs[i]->title);
        printf("\tArtist: %s\n", playlist->songs[i]->artist);
        printf("\tReleased: %d\n", playlist->songs[i]->year);
        printf("\tStreams: %d\n", playlist->songs[i]->streams);
    }

    printf("choose a song to delete, or 0 to quit:\n");
    int songToDeleteChoice;
    scanf("%d", &songToDeleteChoice);
    if (songToDeleteChoice > 0 && songToDeleteChoice <= playlist->songsNum)
    {
        freeSong(playlist->songs[songToDeleteChoice - 1]);
        for (int i = songToDeleteChoice - 1; i < playlist->songsNum - 1; i++)
        {
            playlist->songs[i] = playlist->songs[i + 1];
        }
        playlist->songsNum--;
    }
    else
    {
        printf("Invalid choice, please try again.\n");
    }
}

void addSong(Playlist *playlist)
{
    Song *song = (Song *)malloc(sizeof(Song));
    if (!song)
        exit(1);

    printf("Enter song's details\n");
    printf("Title:\n");
    char *title = getStringInput();
    song->title = (char *)malloc(strlen(title) + 1);
    if (!song->title)
    {
        free(title);
        free(song);
        exit(1);
    }
    strcpy(song->title, title);
    free(title);

    printf("Artist:\n");
    char *artist = getStringInput();
    song->artist = (char *)malloc(strlen(artist) + 1);
    if (!song->artist)
    {
        free(artist);
        free(song->title);
        free(song);
        exit(1);
    }

    strcpy(song->artist, artist);
    free(artist);

    printf("Year of release:\n");
    int year;
    scanf("%d", &year);
    song->year = year;
    if (!song->year)
    {
        free(song->artist);
        free(song->title);
        free(song);
        exit(1);
    }
    song->year = year;

    printf("Lyrics:\n");
    char *lyrics = getStringInput();
    song->lyrics = (char *)malloc(strlen(lyrics) + 1);
    if (!song->lyrics)
    {
        free(lyrics);
        free(song->artist);
        free(song->title);
        free(song);
        exit(1);
    }
    strcpy(song->lyrics, lyrics);
    free(lyrics);

    song->streams = 0;

    Song **temp = (Song **)realloc(playlist->songs, (playlist->songsNum + 1) * sizeof(Song *));
    if (!temp)
    {
        free(song->title);
        free(song);
        exit(1);
    }
    playlist->songs = temp;
    playlist->songs[playlist->songsNum] = song;
    playlist->songsNum++;
}

void addPlaylist(Playlist **playlists, int *playlistCount, char *playlistName)
{
    Playlist *temp = (Playlist *)realloc(*playlists, (*playlistCount + 1) * sizeof(Playlist));
    if (!temp)
    {
        free(playlistName);
        printf("Memory allocation failed!\n");
        exit(1);
    }
    *playlists = temp;

    (*playlists)[*playlistCount].songs = NULL;
    (*playlists)[*playlistCount].songsNum = 0;

    if (playlistName)
    {
        (*playlists)[*playlistCount].name = (char *)malloc(strlen(playlistName) + 1);
        if (!(*playlists)[*playlistCount].name)
        {
            free(playlistName);
            exit(1);
        }

        strcpy((*playlists)[*playlistCount].name, playlistName);
    }
    else
    {
        (*playlists)[*playlistCount].name = NULL;
    }

    free(playlistName);
    (*playlistCount)++;
}

void removePlaylist(Playlist *playlists, int playlistCount) {
    
}

void printPlaylistsMenu(Playlist **playlists, int *playlistsCount)
{
    while (1)
    {
        int choice;
        printf("Please Choose:\n");
        printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Choose a playlist:\n");
            watchPlaylists(*playlists, *playlistsCount);
            break;
        case 2:
            printf("Enter playlist's name:\n");
            char *name = getStringInput();
            addPlaylist(playlists, playlistsCount, name);
            break;
        case 3:
            removePlaylist(*playlists, *playlistsCount);
            break;
        case 4:
            // exit
            return;
        default:
            printf("Invalid choice, please try again.\n");
            break;
        }
    }
}

int main()
{
    Playlist *playlists = NULL;
    int playlistsCount = 0;

    printPlaylistsMenu(&playlists, &playlistsCount);
    for (int i = 0; i < playlistsCount; i++)
    {
        freePlaylist(&playlists[i]);
    }

    free(playlists);

    printf("Goodbye!\n");
    return 0;
}
