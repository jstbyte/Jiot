# Instructions!

Use Iot Mqtt Panel (Android App).

```ts
/* data/config.json  */ {
  "identity": "secrat.domain.ext",
  "wlanSSID": "ssid@sta",
  "wlanPASS": "pass@sta",
  "hostNAME": "dev@name",
  "mqttHOST": "broker.emqx.io",
  "mqttPORT": 8883 // 1883;
}

/* ( CA Cert Embaded Into Program Memory in certs.h 'maybe change in future' ) */
/* TOPIC FORMAT: `{SECRAT}/${DEVICE | *}/{req|res}/{SERVICES}#` */
/* SONOFF:: Pin No. 13 Used By IR Reciver Sensor */
/* data/sonoff.txt = `4:1;5:1;12:1;14:1` (optional) */

const topics = [ // Regex [...];
  {topic: 'power', payload: ``,}
  {topic: 'update', req: `?{URL}`},
  {topic:'sonoff/[index]', req: `[0|1|>1]`}
  {topic: 'sonoff', req: `[index]:[state];...}`, res:`{[index]:[state]}`}
]

// TODO: Future:: https://api.github.com/repos/jstbyte/Jiot/releases/latest;
```
