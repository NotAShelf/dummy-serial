# Dummy Serial

> Simulates a serial port that continuously sends dummy data for PM2.5 and PM10. It uses a pseudoterminal (pty) to simulate the serial port. Designed to provide dummy data in the absence of a sensor for [air-quality-monitor](https://github.com/NotAShelf/air-quality-monitor)

## Usage

1. Compile & start the program.

```console
gcc -o dummy_serial main.c
./dummy_serial
```

2. Start air-quality-monitor.
3. Navigate to http://localhost:8080 and observe the chart.

## License

This project is licensed under the GPL3 License. See [LICENSE](LICENSE) for more details.

## Contributing

PRs welcome.
