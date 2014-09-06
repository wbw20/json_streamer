APOLLO
=============

by Carbon Origins

This is the operating system for Apollo.  The following api can be used over USB.

## API

```code
{
  "command": "temperature"
}
```

```code
{
  "command": "pressure"
}
```

## Expirimental API

```code
{
  "command": "log",
  "sensors": [
    "temperature",
    "humidity",
    "pressure"
  ],
  "frequency": 100
}
```