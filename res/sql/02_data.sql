INSERT INTO users (name) VALUES ('Agustin');
INSERT INTO users (name) VALUES ('Alejandro');
INSERT INTO users (name, parent_id) VALUES ('Fernando', 1);
INSERT INTO users (name) VALUES ('Testing');

INSERT into posts (description, image_url, owner_id) VALUES ('1st Post', 'TestImage.png', 2);
INSERT into posts (description, image_url, owner_id) VALUES ('2nd Post', 'TestImage.png', 3);
INSERT into posts (description, image_url, owner_id) VALUES ('3rd Post', 'TestImage.png', 1);
INSERT into posts (description, image_url, owner_id, parent_id) VALUES ('4th Post', 'TestImage.png', 3, 3);

INSERT INTO likes (user_id, post_id) VALUES (1, 1);
INSERT INTO likes (user_id, post_id) VALUES (1, 2);
INSERT INTO likes (user_id, post_id) VALUES (2, 3);
INSERT INTO likes (user_id, post_id) VALUES (2, 4);
INSERT INTO likes (user_id, post_id) VALUES (3, 4);
INSERT INTO likes (user_id, post_id) VALUES (3, 1);
