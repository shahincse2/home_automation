import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'package:shared_preferences/shared_preferences.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Home Automation',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.teal),
        useMaterial3: true,
      ),
      home: const MyHomePage(title: 'WiFiManager IoT Control'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key, required this.title});
  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  bool isSwitched = false;
  String? ipAddress;
  TextEditingController ipController = TextEditingController();

  @override
  void initState() {
    super.initState();
    loadIPAddress();
  }

  Future<void> loadIPAddress() async {
    final prefs = await SharedPreferences.getInstance();
    ipAddress = prefs.getString('ipAddress');
    if (ipAddress != null) {
      await getLedStatus();
      setState(() {}); // UI রিফ্রেশ
    }
  }

  Future<void> saveIPAddress(String ip) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setString('ipAddress', ip);
    ipAddress = ip;
    await getLedStatus();
    setState(() {});
  }

  Future<void> toggleLight() async {
    if (ipAddress == null) return;
    try {
      final response = await http.get(Uri.parse('http://$ipAddress/led_toggle'));
      if (response.statusCode == 200) {
        await getLedStatus();
      } else {
        print('Failed to toggle LED');
      }
    } catch (e) {
      print('Error: $e');
    }
  }

  Future<void> getLedStatus() async {
    if (ipAddress == null) return;
    try {
      final response = await http.get(Uri.parse('http://$ipAddress/led_status'));
      if (response.statusCode == 200) {
        setState(() {
          isSwitched = response.body.trim() == 'ON';
        });
      } else {
        print('Failed to get status');
      }
    } catch (e) {
      print('Status error: $e');
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text(widget.title)),
      body: ipAddress == null
          ? Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          children: [
            const Text('Enter NodeMCU IP Address:'),
            TextField(
              controller: ipController,
              decoration: const InputDecoration(
                labelText: 'e.g. 192.168.0.103',
              ),
              keyboardType: TextInputType.number,
            ),
            const SizedBox(height: 16),
            ElevatedButton(
              onPressed: () {
                if (ipController.text.isNotEmpty) {
                  saveIPAddress(ipController.text.trim());
                }
              },
              child: const Text('Save & Connect'),
            ),
          ],
        ),
      )
          : Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Text(
              isSwitched ? 'LED ON' : 'LED OFF',
              style: const TextStyle(fontSize: 24),
            ),
            const SizedBox(height: 10),
            Switch(
              value: isSwitched,
              onChanged: (_) async {
                await toggleLight();
                await getLedStatus();
              },
            ),
            const SizedBox(height: 20),
            ElevatedButton(
              onPressed: () async {
                final prefs = await SharedPreferences.getInstance();
                await prefs.remove('ipAddress');
                setState(() {
                  ipAddress = null;
                  isSwitched = false;
                });
              },
              child: const Text('Change IP'),
            ),
          ],
        ),
      ),
    );
  }
}
