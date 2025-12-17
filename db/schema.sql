-- Student Attendance System Database Schema
-- SQLite version

CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL UNIQUE,
    role TEXT NOT NULL DEFAULT 'user',
    password_hash TEXT NOT NULL,
    salt TEXT NOT NULL,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX IF NOT EXISTS idx_users_username ON users(username);

-- Students table
CREATE TABLE IF NOT EXISTS students (
    student_id TEXT PRIMARY KEY NOT NULL,
    name TEXT NOT NULL,
    class_name TEXT NOT NULL,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- Create index for class queries
CREATE INDEX IF NOT EXISTS idx_students_class ON students(class_name);
CREATE INDEX IF NOT EXISTS idx_students_name ON students(name);

-- Attendances table
CREATE TABLE IF NOT EXISTS attendances (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    student_id TEXT NOT NULL,
    date TEXT NOT NULL,
    status TEXT NOT NULL CHECK(status IN ('present', 'absent', 'late', 'early_leave', 'sick_leave', 'personal_leave')),
    remark TEXT DEFAULT '',
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (student_id) REFERENCES students(student_id) ON DELETE CASCADE
);

-- Create indexes for common queries
CREATE INDEX IF NOT EXISTS idx_attendances_student ON attendances(student_id);
CREATE INDEX IF NOT EXISTS idx_attendances_date ON attendances(date);
CREATE INDEX IF NOT EXISTS idx_attendances_status ON attendances(status);
CREATE INDEX IF NOT EXISTS idx_attendances_student_date ON attendances(student_id, date);

-- Trigger to update updated_at on students
CREATE TRIGGER IF NOT EXISTS update_students_timestamp
AFTER UPDATE ON students
BEGIN
    UPDATE students SET updated_at = CURRENT_TIMESTAMP WHERE student_id = NEW.student_id;
END;

CREATE TRIGGER IF NOT EXISTS update_users_timestamp
AFTER UPDATE ON users
BEGIN
    UPDATE users SET updated_at = CURRENT_TIMESTAMP WHERE id = NEW.id;
END;

-- Trigger to update updated_at on attendances
CREATE TRIGGER IF NOT EXISTS update_attendances_timestamp
AFTER UPDATE ON attendances
BEGIN
    UPDATE attendances SET updated_at = CURRENT_TIMESTAMP WHERE id = NEW.id;
END;

-- Insert sample data
INSERT OR IGNORE INTO students (student_id, name, class_name) VALUES
    ('2024001', '张三', '人文2401班'),
    ('2024002', '李四', '人文2401班'),
    ('2024003', '王五', '人文2401班'),
    ('2024004', '赵六', '人文2402班'),
    ('2024005', '钱七', '人文2402班'),
    ('2024006', '孙八', '人文2402班'),
    ('2024007', '周九', '人文2403班'),
    ('2024008', '吴十', '人文2403班');

INSERT OR IGNORE INTO attendances (student_id, date, status, remark) VALUES
    ('2024001', '12-15', 'present', ''),
    ('2024002', '12-15', 'present', ''),
    ('2024003', '12-15', 'late', '迟到5分钟'),
    ('2024004', '12-15', 'absent', ''),
    ('2024005', '12-15', 'present', ''),
    ('2024006', '12-15', 'sick_leave', '感冒'),
    ('2024007', '12-15', 'present', ''),
    ('2024008', '12-15', 'personal_leave', '家中有事');
