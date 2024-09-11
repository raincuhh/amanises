# Types

## brief
This documentation provides an overview of the different data types available in the language. It includes signed integers, unsigned integers, floating-point numbers, and other basic types.

| **Type** | **Description**         | **Value Range**                    |
|----------|-------------------------|-----------------------------------|
| `i8`     | Signed 8-bit integer    | -128 to 127                       |
| `i16`    | Signed 16-bit integer   | -32,768 to 32,767                 |
| `i32`    | Signed 32-bit integer   | -2,147,483,648 to 2,147,483,647   |
| `i64`    | Signed 64-bit integer   | -9,223,372,036,854,776 to 9,223,372,036,854,775,807 |
| `i128`   | Signed 128-bit integer  | -2^127 to 2^127-1                |
| `u8`     | Unsigned 8-bit integer  | 0 to 255                          |
| `u16`    | Unsigned 16-bit integer | 0 to 65,535                       |
| `u32`    | Unsigned 32-bit integer | 0 to 4,294,967,295               |
| `u64`    | Unsigned 64-bit integer | 0 to 18,446,744,073,709,551,615 |
| `u128`   | Unsigned 128-bit integer| 0 to 2^128-1                     |
| `f32`    | 32-bit floating-point   | ±1.7E−308 to ±3.4E+38            |
| `f64`    | 64-bit floating-point   | ±5.0E−324 to ±1.7E+308          |
| `char`   | Character type          | Depends on encoding (e.g., ASCII, UTF-8) |
| `string` | String type             | Variable length, depends on implementation |
| `bool`   | Boolean type            | true or false                      |
| `void`   | Void type               | Represents no value or type       |