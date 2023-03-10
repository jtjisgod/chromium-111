PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE meta(key LONGVARCHAR NOT NULL UNIQUE PRIMARY KEY, value LONGVARCHAR);
INSERT INTO meta VALUES('mmap_status','-1');
INSERT INTO meta VALUES('last_compatible_version','1');
INSERT INTO meta VALUES('version','2');
CREATE TABLE per_origin_mapping(context_origin TEXT NOT NULL PRIMARY KEY,creation_time INTEGER NOT NULL,length INTEGER NOT NULL) WITHOUT ROWID;
INSERT INTO "per_origin_mapping" VALUES ('http://google.com',13266954476192362,2);
INSERT INTO "per_origin_mapping" VALUES ('http://youtube.com',13266954593856733,1);
INSERT INTO "per_origin_mapping" VALUES ('http://chromium.org',13268941676192362,3);
INSERT INTO "per_origin_mapping" VALUES ('http://gv.com',13268941793856733,1);
INSERT INTO "per_origin_mapping" VALUES ('http://abc.xyz',13269481776356965,2);
INSERT INTO "per_origin_mapping" VALUES ('http://withgoogle.com',13269545986263676,1);
INSERT INTO "per_origin_mapping" VALUES ('http://waymo.com',13269546064355176,1);
INSERT INTO "per_origin_mapping" VALUES ('http://google.org',13269546476192362,4);
INSERT INTO "per_origin_mapping" VALUES ('http://growwithgoogle.com',13269546593856733,3);
CREATE TABLE budget_mapping(id INTEGER NOT NULL PRIMARY KEY,context_origin TEXT NOT NULL,time_stamp INTEGER NOT NULL,bits_debit REAL NOT NULL);
INSERT INTO "budget_mapping" VALUES (1,'http://google.com',13266954476192332,1.0);
INSERT INTO "budget_mapping" VALUES (2,'http://google.com',13266954476192344,2.0);
INSERT INTO "budget_mapping" VALUES (3,'http://google.com',13266954476192362,1.1);
INSERT INTO "budget_mapping" VALUES (4,'http://youtube.com',13266954593856693,1.3);
INSERT INTO "budget_mapping" VALUES (5,'http://chromium.org',13268941676192362,1.5);
INSERT INTO "budget_mapping" VALUES (6,'http://youtube.com',13266954593856733,2.3);
INSERT INTO "budget_mapping" VALUES (7,'http://gv.com',13268941793856733,3.7);
INSERT INTO "budget_mapping" VALUES (8,'http://abc.xyz',13269481776356845,2.2);
INSERT INTO "budget_mapping" VALUES (9,'http://withgoogle.com',13269545986263676,1);
INSERT INTO "budget_mapping" VALUES (10,'http://abc.xyz',13269481776356905,1.1);
INSERT INTO "budget_mapping" VALUES (11,'http://waymo.com',13269546064355176,4.2);
INSERT INTO "budget_mapping" VALUES (12,'http://abc.xyz',13269481776356965,2.0);
INSERT INTO "budget_mapping" VALUES (13,'http://google.org',13269546476192362,4);
INSERT INTO "budget_mapping" VALUES (14,'http://growwithgoogle.com',13269546593856733,1.2);
CREATE TABLE IF NOT EXISTS "values_mapping"(context_origin TEXT NOT NULL,key TEXT NOT NULL,value TEXT,last_used_time INTEGER NOT NULL,PRIMARY KEY(context_origin,key)) WITHOUT ROWID;
INSERT INTO "values_mapping" VALUES ('http://google.com','key1','value1',13312097333991364);
INSERT INTO "values_mapping" VALUES ('http://google.com','key2','value2',13313037427966159);
INSERT INTO "values_mapping" VALUES ('http://youtube.com','visited','1111111',13313037435619704);
INSERT INTO "values_mapping" VALUES ('http://chromium.org','a','',13313037416916308);
INSERT INTO "values_mapping" VALUES ('http://chromium.org','b','hello',13312097333991364);
INSERT INTO "values_mapping" VALUES ('http://chromium.org','c','goodbye',13312353831182651);
INSERT INTO "values_mapping" VALUES ('http://gv.com','cookie','13268941793856733',13313037487092131);
INSERT INTO "values_mapping" VALUES ('http://abc.xyz','seed','387562094',13269481776356965);
INSERT INTO "values_mapping" VALUES ('http://abc.xyz','bucket','1276',13269481776356965);
INSERT INTO "values_mapping" VALUES ('http://withgoogle.com','count','389',13269545986263676);
INSERT INTO "values_mapping" VALUES ('http://waymo.com','key','value',13269546064355176);
INSERT INTO "values_mapping" VALUES ('http://google.org','1','ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff',13269546476192362);
INSERT INTO "values_mapping" VALUES ('http://google.org','2',';',13269546476192362);
INSERT INTO "values_mapping" VALUES ('http://google.org','#','[]',13269546476192362);
INSERT INTO "values_mapping" VALUES ('http://google.org','ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff','k',13269546476192362);
INSERT INTO "values_mapping" VALUES ('http://growwithgoogle.com','_','down',13269546593856733);
INSERT INTO "values_mapping" VALUES ('http://growwithgoogle.com','<','left',13269546593856733);
INSERT INTO "values_mapping" VALUES ('http://growwithgoogle.com','>','right',13269546593856733);
CREATE INDEX budget_mapping_origin_time_stamp_idx ON budget_mapping(context_origin,time_stamp);
CREATE INDEX values_mapping_last_used_time_idx ON values_mapping(last_used_time);
CREATE INDEX per_origin_mapping_creation_time_idx ON per_origin_mapping(creation_time);
COMMIT;
