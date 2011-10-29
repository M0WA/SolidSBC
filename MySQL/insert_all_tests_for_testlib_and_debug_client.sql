SET FOREIGN_KEY_CHECKS=0;

DELETE FROM `SolidSBC`.`ClientHistory`;
ALTER TABLE `SolidSBC`.`ClientHistory` AUTO_INCREMENT = 0;

DELETE FROM `SolidSBC`.`ClientsConfigs`;
ALTER TABLE `SolidSBC`.`ClientsConfigs` AUTO_INCREMENT = 0;

DELETE FROM `SolidSBC`.`Configs`;
ALTER TABLE `SolidSBC`.`Configs` AUTO_INCREMENT = 0;

DELETE FROM `SolidSBC`.`Clients`;
ALTER TABLE `SolidSBC`.`Clients` AUTO_INCREMENT = 0;

SET FOREIGN_KEY_CHECKS=1;

-- --------------------------------------------------

INSERT INTO `SolidSBC`.`Configs` ( `ID`, `xmlConfig` ) 
	VALUES 
  ( 1, '<Test><MaxMem>1048576</MaxMem><MinMem>1</MinMem><Randomize>1</Randomize><TestName>Memory-Test</TestName><TransmitData>1</TransmitData></Test>' ),
  ( 2, '<Test><AdditionCnt>1000</AdditionCnt><AutoFixMultiplier>3</AutoFixMultiplier><DivisionCnt>1000</DivisionCnt><FixMultiplierTreshold>0.5</FixMultiplierTreshold><TestName>CPU-Measure-Test</TestName><TransmitData>1</TransmitData></Test>' ),
  ( 3, '<Test><MaxRand>20</MaxRand><Randomize>1</Randomize><SleepMS>5.000000</SleepMS><TestName>CPU-Test</TestName><ThreadCnt>2</ThreadCnt></Test>' ),
  ( 4, '<Test><RandomRead>1</RandomRead><RandomWrite>1</RandomWrite><ReadMax>10485760</ReadMax><ReadWriteDelay>20</ReadWriteDelay><TestName>Harddrive-Test</TestName><TransmitData>1</TransmitData><WriteMax>10485760</WriteMax></Test>' ),
  ( 5, '<Test><Host>127.0.0.1</Host><Interval>60000</Interval><PayloadSize>32</PayloadSize><TTL>1000</TTL><TestName>Net-Ping-Test</TestName><TransmitData>1</TransmitData></Test>' ),
  ( 6, '<Test><Host>127.0.0.1</Host><Interval>60000</Interval><Port>80</Port><TTL>1000</TTL><TestName>Net-TCP-Test</TestName><TransmitData>1</TransmitData></Test>' ),
  
  -- insert non default test
  ( 7, '<Test><MaxMem>4096000</MaxMem><MinMem>1</MinMem><Randomize>1</Randomize><TestName>Memory-Test</TestName><TransmitData>1</TransmitData></Test>' );

-- --------------------------------------------------

INSERT INTO `SolidSBC`.`Clients` ( `ID`, `uuid`, `name` )
	VALUES 
  ( 1, '374ce507902419b-3ef865b4-490f-aaaf-ceccb44d3b15ca6d3d07419c' , 'MASTERMIND (Win32 Debug)' ),
  ( 2, '3064ab4bf55334ec-79399f0b-4537-ad9d-3ed450c034ad47a079ef223c', 'MASTERMIND (Win64 Debug)' ),
  ( 3, 'c0ce1e50532a1b4c-dc66da3c-4b13-bae2-af1b64896d1662e51a298e1' , 'TOY (Win32 Debug)' ),
  ( 4, 'c90c38506732b4ef-7ba04d65-4a03-9f45-14113fc1dcea1b848c92d46' , 'VM Ubuntu SolidSBCLinux' );

-- --------------------------------------------------

 -- tests for clients
INSERT INTO `SolidSBC`.`ClientsConfigs` ( `CLIENT_ID`, `CONFIG_ID` )
	VALUES 
  ( 1,7 ),
  ( 1,2 ),
  ( 1,3 ),
  ( 1,4 ),
  ( 1,5 ),
  ( 1,6 ),
  ( 2,7 ),
  ( 2,2 ),
  ( 2,3 ),
  ( 2,4 ),
  ( 3,1 ),
  ( 4,1 );

-- --------------------------------------------------
-- just some test entries, not needed for productional
/*
INSERT INTO `SolidSBC`.`ClientHistory` ( `CLIENT_ID`, `action`, `actiontime` )
	VALUES
		(1,1, NOW()),
		(1,2, NOW()),
		(1,1, NOW()),
		(1,2, NOW()),
		(2,1, NOW()),
		(2,2, NOW()),
		(2,1, NOW()),
		(2,2, NOW());
*/