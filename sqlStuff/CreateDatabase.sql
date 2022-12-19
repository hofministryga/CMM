-- SQLite
CREATE TABLE IF NOT EXISTS MESSAGES (
    MessageID INTEGER PRIMARY KEY,
    Title TEXT NOT NULL,
    VideoLink TEXT,
    AudioLink TEXT,
    Description TEXT,
    DateRecorded DATE,
    ExpireDate DATE
);

INSERT INTO MESSAGES (Title, VideoLink, AudioLink, Description, DateRecorded)
    VALUES ("TestTitle1", "127.0.0.1", "127.0.0.1", "Here is a test description. It has no line breaks.", "1970-01-01");

INSERT INTO MESSAGES (Title, VideoLink, AudioLink, Description, DateRecorded)
    VALUES ("TestTitle2", "127.0.0.1", "127.0.0.1", "Here is a test description.
     It has a line breaks.", "1970-01-01");

INSERT INTO MESSAGES (Title, VideoLink, AudioLink, Description, DateRecorded)
    VALUES ("TestTitle4", "127.0.0.1", "127.0.0.1", "Here is a test description. 
    
    It has 2 line breaks.", "1970-01-01");

INSERT INTO MESSAGES (Title, VideoLink, AudioLink, Description, DateRecorded)
    VALUES ("TestTitle7", "127.0.0.1", "127.0.0.1", "Here is a test description. 
    
    
    It has 3 line breaks.", "1970-01-01");