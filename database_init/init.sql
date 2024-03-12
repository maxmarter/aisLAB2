use archdb; 

CREATE TABLE IF NOT EXISTS `User` (
    `id` INT NOT NULL AUTO_INCREMENT,
    `first_name` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
    `last_name` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
    `email` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL,
    `phone_number` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL,
    `login` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL,
    `password` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL,
    PRIMARY KEY (`id`),
    INDEX `fn` (`first_name`),
    INDEX `ln` (`last_name`));

CREATE TABLE IF NOT EXISTS `Wall` (`id` INT NOT NULL AUTO_INCREMENT,
                        `user_id` INT NOT NULL,
                        `post` VARCHAR(256) NOT NULL,
                        `post_id` INT NOT NULL,
                        PRIMARY KEY (`id`),
                        FOREIGN KEY (`user_id`)  REFERENCES `User` (`id`));

CREATE TABLE IF NOT EXISTS `Message` (`id` INT NOT NULL AUTO_INCREMENT,
                        `from_user_id` INT NOT NULL,
                        `to_user_id`INT NOT NULL,
                        `message` VARCHAR(256) NOT NULL,
                        FOREIGN KEY (`to_user_id`)  REFERENCES `User` (`id`),
                        PRIMARY KEY (`id`));

INSERT INTO `User` (`first_name`, `last_name`, `login`, `password`, `email`) values
('Ivan', 'Ivanov', 'ivanivanov', '123456234', 'ivanov@example.com'),
('Maria', 'Petrova', 'mpetrova', '1234234','petrova@example.net'  ),
('Sergey', 'Sidorov','ssidorov', '1232342', 'sidorov@example.org' ),
('Anna', 'Kuznetsova', 'akuznetsova', '1234234', 'kuznetsova@example.com'),
('Evgeny', 'Alekseev', 'ealekseev', '2234234', 'alekseev@example.info'),
('Tatyana', 'Vasilieva', 'tvasilieva', '324234', 'vasilieva@example.net'),
('Nikolay', 'Nikolaev', 'nikolaev1', '1234', 'nikolaev@example.org'), 
('Olga', 'Smirnova', 'smirnova1', '567834', 'smirnova@example.com'),
('Andrey', 'Andreev', 'andreev1', '904353', 'andreev@example.info'),
('Elena', 'Kozlova', 'kozlova1', '0345345435', 'kozlova@example.net'),
('Victor', 'Fedorov', 'fedorov1', '1145435', 'fedorov@example.org'),
('Roman', 'Romanov', 'romanov1', '2212213', 'romanov@example.com'),
('Anastasia', 'Novikova', 'anastas', '123123', 'novikova@example.net'),
('Boris', 'Borisov', 'borboris', '203203', 'borisov@example.org'),
('Galina', 'Grigorieva', 'ggrigor', '203012301', 'grigoreva@example.com'),
('Dmitry', 'Dmitriev', 'dmitrydm', '203120301', 'dmitriev@example.info'),
('Evgenia', 'Ezhova', 'ezevia', '3090193021', 'ezhova@example.net');

INSERT INTO `Wall` (`user_id`, `post`, `post_id`) VALUES
(1, 'First post on this brand new platform!!!',
    1
);



INSERT INTO `Message` (from_user_id, to_user_id, `message`) values
(1, 2, 'First message'),
(
    2,
    1,
    'Hooray!'
    
),
(
    2,
    1,
    'Wuzzup?'
    
),
(
    1,
    3,
    'Hello! This is an automatic message'
    
),
(
    3,
    1,
    'Hello, automatic message'
    
),
(
    4,
    5,
    'Tommorow is my turn to go to the mall'
    
),
(
    5,
    1,
    'I would like to eat some pop-corn...'
    
),
(
    2,
    4,
    '. . .'
    
),
(
    7,
    1,
    'I wish I could have some rest'
    
),
(
    1,
    7,
    'You will learn by the numbers, I will teach you!'
    
);