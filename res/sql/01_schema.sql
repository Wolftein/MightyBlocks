 DROP TABLE IF EXISTS likes; 
 DROP TABLE IF EXISTS posts;
 DROP TABLE IF EXISTS users;

CREATE TABLE IF NOT EXISTS users
(
    id bigint NOT NULL GENERATED BY DEFAULT AS IDENTITY ( INCREMENT 1 START 1 MINVALUE 1 MAXVALUE 9223372036854775807 CACHE 1 ),
    created_at bigint DEFAULT (EXTRACT(epoch FROM now()) * (1000)::numeric),
    name character varying(64) COLLATE pg_catalog."default" NOT NULL,
    parent_id bigint,
    CONSTRAINT users_pkey PRIMARY KEY (id),
    CONSTRAINT users_name_key UNIQUE (name),
    CONSTRAINT users_parent_id_fkey FOREIGN KEY (parent_id)
        REFERENCES users (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION
);

CREATE TABLE IF NOT EXISTS posts
(
    id bigint NOT NULL GENERATED BY DEFAULT AS IDENTITY ( INCREMENT 1 START 1 MINVALUE 1 MAXVALUE 9223372036854775807 CACHE 1 ),
    created_at bigint DEFAULT (EXTRACT(epoch FROM now()) * (1000)::numeric),
    description character varying(1500) COLLATE pg_catalog."default" NOT NULL,
    image_url character varying(512) COLLATE pg_catalog."default" NOT NULL,
    owner_id bigint NOT NULL,
    parent_id bigint,
    CONSTRAINT posts_pkey PRIMARY KEY (id),
    CONSTRAINT posts_owner_id_fkey FOREIGN KEY (owner_id)
        REFERENCES users (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION,
    CONSTRAINT posts_parent_id_fkey FOREIGN KEY (parent_id)
        REFERENCES posts (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION
);

CREATE TABLE IF NOT EXISTS likes
(
    id bigint NOT NULL GENERATED BY DEFAULT AS IDENTITY ( INCREMENT 1 START 1 MINVALUE 1 MAXVALUE 9223372036854775807 CACHE 1 ),
    user_id bigint NOT NULL,
    post_id bigint NOT NULL,
    CONSTRAINT uq_likes UNIQUE (user_id, post_id),
    CONSTRAINT likes_post_id_fkey FOREIGN KEY (post_id)
        REFERENCES posts (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION,
    CONSTRAINT likes_user_id_fkey FOREIGN KEY (user_id)
        REFERENCES users (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION
);