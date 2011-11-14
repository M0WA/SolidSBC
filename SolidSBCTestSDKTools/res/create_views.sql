SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL';

-- -----------------------------------------------------
-- View `SolidSBC`.`ClientConfigInfo`
-- -----------------------------------------------------
DROP VIEW IF EXISTS `SolidSBC`.`ClientConfigInfo` ;
DROP TABLE IF EXISTS `SolidSBC`.`ClientConfigInfo`;
USE `SolidSBC`;
CREATE  OR REPLACE VIEW `SolidSBC`.`ClientConfigInfo` AS 
SELECT
    `cc`.`CLIENT_ID` AS `client id`,
    
    GetClientName(`cc`.`CLIENT_ID`) AS `client`,
    
    `xmlConfig` AS `xml config`
FROM       `ClientsConfigs` AS `cc`
INNER JOIN `Clients` AS `cli` ON ( `cli`.`ID` = `cc`.`CLIENT_ID` )
INNER JOIN `Configs` AS `cfg` ON ( `cfg`.`ID` = `cc`.`CONFIG_ID` )
GROUP BY `cc`.`CLIENT_ID`;


-- -----------------------------------------------------
-- View `SolidSBC`.`ClientHistoryInfo`
-- -----------------------------------------------------
DROP VIEW IF EXISTS `SolidSBC`.`ClientHistoryInfo` ;
DROP TABLE IF EXISTS `SolidSBC`.`ClientHistoryInfo`;
USE `SolidSBC`;
CREATE  OR REPLACE VIEW `SolidSBC`.`ClientHistoryInfo` AS
SELECT
    `ch`.`ID`,

    `c`.`ID` AS `client id`,

    GetClientName(`c`.`ID`) AS `client`,
    
    CASE 
        WHEN `ch`.`action` = 1 THEN 'login'
        WHEN `ch`.`action` = 2 THEN 'logout'
        ELSE 'n/a'
    END AS `action`,
    
    `ch`.`actiontime` AS `datetime`
    
FROM `ClientHistory` AS `ch`
INNER JOIN `Clients` AS `c`  ON ( `ch`.`CLIENT_ID` = `c`.`ID` )
ORDER BY `ch`.`CLIENT_ID`;


-- -----------------------------------------------------
-- View `SolidSBC`.`ClientsSessions`
-- -----------------------------------------------------
DROP VIEW IF EXISTS `SolidSBC`.`ClientsSessions` ;
DROP TABLE IF EXISTS `SolidSBC`.`ClientsSessions`;
USE `SolidSBC`;
CREATE  OR REPLACE VIEW `SolidSBC`.`ClientsSessions` AS
SELECT

    `ch_start`.`ID` AS `start ID`,
    
    IF (
        `ch_end`.`ID` IS NOT NULL,
        `ch_end`.`ID`,
        IF (
            `ch_end_last`.`ID` IS NOT NULL,
            `ch_end_last`.`ID`,
            ''
        )
    ) AS `end ID`,


    `ch_start`.`CLIENT_ID`                AS `CLIENT_ID`,
    GetClientName(`ch_start`.`CLIENT_ID`) AS `client`,
    
    `ch_start`.`actiontime` AS `start time`,
        
    IF (
        `ch_end`.`actiontime` IS NOT NULL,
        `ch_end`.`actiontime`,
        IF (
            `ch_end_last`.`actiontime` IS NOT NULL,
            `ch_end_last`.`actiontime`,
            'active'
        )
    )
    AS `end time`
    
FROM `SolidSBC`.`ClientHistory` AS `ch_start`
LEFT JOIN `SolidSBC`.`ClientHistory` AS `ch_next`     ON ( `ch_next`.`CLIENT_ID`     = `ch_start`.`CLIENT_ID` AND `ch_start`.`ID` < `ch_next`.`ID`     AND `ch_next`.`action` = 1 )
LEFT JOIN `SolidSBC`.`ClientHistory` AS `ch_end`      ON ( `ch_end`.`CLIENT_ID`      = `ch_start`.`CLIENT_ID` AND `ch_start`.`ID` < `ch_end`.`ID`      AND `ch_end`.`ID` < `ch_next`.`ID` AND `ch_end`.`action` = 2)
LEFT JOIN `SolidSBC`.`ClientHistory` AS `ch_end_last` ON ( `ch_end_last`.`CLIENT_ID` = `ch_start`.`CLIENT_ID` AND `ch_start`.`ID` < `ch_end_last`.`ID` AND `ch_end_last`.`action` = 2 AND (`ch_next`.`ID` > `ch_end_last`.`ID` OR (`ch_next`.`ID` IS NULL) ))
WHERE `ch_start`.`action` = 1;

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;