CREATE FUNCTION `SolidSBC`.`GetClientName` (
    clientID BIGINT
    ) RETURNS VARCHAR(1024)

BEGIN
    DECLARE `retval` VARCHAR(1024);
    
    SET `retval` = (
        SELECT 
        CASE
            WHEN `name` IS NOT NULL THEN `name`
            ELSE `uuid`
        END
        FROM `SolidSBC`.`Clients` WHERE `ID` = `clientID`
    );
    
    RETURN `retval`;
END
;