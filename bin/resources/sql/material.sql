<?xml version="1.0" encoding="UTF-8"?>
<sqls namespace="Material">
    <define id="fields">id, username, password, email, mobile</define>

    <sql id="findById">
        SELECT <include defineId="fields"/> FROM user WHERE id=%1
    </sql>

    <sql id="findAll">
        SELECT id, username, password, email, mobile FROM user
    </sql>

    <sql id="insert">
        INSERT INTO user (username, password, email, mobile)
        VALUES (:username, :password, :email, :mobile)
    </sql>

    <sql id="update">
        UPDATE user SET username=:username, password=:password,
            email=:email, mobile=:mobile
        WHERE id=:id
    </sql>
</sqls>
