CREATE FUNCTION register_user(id char(16), uname varchar(40)) RETURNS void
AS $$
DECLARE
    row_count int;
BEGIN
    UPDATE users SET username = uname , last_used_time = CURRENT_DATE WHERE userid = id;
    GET DIAGNOSTICS row_count = ROW_COUNT;
    IF row_count <> 1 THEN 
        INSERT INTO users values(id, uname, CURRENT_DATE);
    END IF;
    RETURN;
END;
$$
language plpgsql;
