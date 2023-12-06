import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;

class SliderComponent extends StatefulWidget {
  final String label;
  final ValueChanged<double> onChangedCallback;

  const SliderComponent({
    Key? key,
    required this.label,
    required this.onChangedCallback,
  }) : super(key: key);

  @override
  _SliderComponentState createState() => _SliderComponentState();
}

class _SliderComponentState extends State<SliderComponent> {
  double _value = 0.0;
  double _previousValue = 0.0;

  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        Row(
          mainAxisAlignment: MainAxisAlignment.spaceBetween,
          children: [
            Text('0.0'),
            SizedBox(
              width: 200,
              child: Text("[${_value.round().toString()}]  ${widget.label}",
                  style: const TextStyle(fontSize: 20)),
            ),
            Text('180.0'),
          ],
        ),
        Slider(
          value: _value,
          min: 0.0,
          max: 180.0,
          divisions: 180,
          onChanged: (value) {
            if (value != _previousValue) {
              setState(() {
                _value = value;
                _previousValue = value;
              });
              widget.onChangedCallback(value);
            }
          },
        ),
      ],
    );
  }
}

class SliderArms extends StatefulWidget {
  const SliderArms({Key? key}) : super(key: key);

  @override
  State<SliderArms> createState() => _SliderArmsState();
}

class _SliderArmsState extends State<SliderArms> {
  String _ipRed = "192.168.4.1";

  Future _api(String command) async {
    var url = Uri.parse('http://$_ipRed/control?$command');
    var response = await http.get(url);
    print('Response status: ${response.statusCode}');
    print('Response body: ${response.body}');
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Brazo robótico con ESP32'),
      ),
      body: Center(
        child: SizedBox(
          height: 700,
          width: 250,
          child: Column(
            mainAxisAlignment: MainAxisAlignment.start,
            children: <Widget>[
              TextField(
                controller: TextEditingController(text: _ipRed),
                decoration: const InputDecoration(
                  border: OutlineInputBorder(),
                  labelText: 'IP',
                ),
                onChanged: (text) {
                  setState(() {
                    _ipRed = text;
                  });
                },
              ),
              const SizedBox(height: 20),
              SliderComponent(
                label: "Garra",
                onChangedCallback: (value) async {
                  final roundedValue1 = value.round();
                  await _api('garra=$roundedValue1');
                },
              ),
              SliderComponent(
                label: "Muñeca Pitch",
                onChangedCallback: (value) {
                  final roundedValue2 = value.round();
                  await _api('muneca_pitch=$roundedValue2');
                },
              ),
              SliderComponent(
                label: "Muñeca Yaw",
                onChangedCallback: (value) {
                  final roundedValue3 = value.round();
                  await _api('muneca_yaw=$roundedValue3');
                },
              ),
              SliderComponent(
                label: "Codo",
                onChangedCallback: (value) {
                  final roundedValue4 = value.round();
                  await _api('codo=$roundedValue4');
                },
              ),
              SliderComponent(
                label: "Antebrazo",
                onChangedCallback: (value) {
                  final roundedValue5 = value.round();
                  await _api('antebrazo=$roundedValue5');
                },
              ),
              SliderComponent(
                label: "Base",
                onChangedCallback: (value) {
                  final roundedValue6 = value.round();
                  await _api('base=$roundedValue6');
                },
              ),
            ],
          ),
        ),
      ),
    );
  }
}
