import React, { useEffect, useState } from 'react';
import mqtt from 'mqtt';

const ValveControl = () => {
  const [client, setClient] = useState(null);
  const [isConnected, setIsConnected] = useState(false);

  useEffect(() => {
    const options = {
      connectTimeout: 4000,
      clientId: 'web_client_' + Math.random().toString(16).substr(2, 8),
      username: 'Yiombi',  // HiveMQ Username
      password: 'Aa12345678',  // HiveMQ Password
    };

    const connectUrl = 'wss://6a862c626ea84addad97efb3a08f02de.s1.eu.hivemq.cloud:8884/mqtt';
    const mqttClient = mqtt.connect(connectUrl, options);

    mqttClient.on('connect', () => {
      console.log('Connected!');
      setIsConnected(true);
    });

    mqttClient.on('error', (err) => {
      console.error('Connection error: ', err);
      mqttClient.end();
    });

    setClient(mqttClient);

    // Clean up
    return () => {
      if (mqttClient) {
        mqttClient.end();
      }
    };
  }, []);

  const publishMessage_1_left = (message) => {
    if (client && isConnected) {
      client.publish('valve/1/left', message);
    }
  };

  const publishMessage_1_right = (message) => {
    if (client && isConnected) {
      client.publish('valve/1/right', message);
    }
  };
  return (
    <div style={{ textAlign: 'center', marginTop: '100px' }}>
      <h1>Valve Control - left</h1>
      <button onClick={() => publishMessage_1_left('ON')} style={{ margin: '10px', padding: '10px 20px' }}>
        Turn ON
      </button>
      <button onClick={() => publishMessage_1_left('OFF')} style={{ margin: '10px', padding: '10px 20px' }}>
        Turn OFF
      </button>
      <h1>Valve Control - right</h1>
      <button onClick={() => publishMessage_1_right('ON')} style={{ margin: '10px', padding: '10px 20px' }}>
        Turn ON
      </button>
      <button onClick={() => publishMessage_1_right('OFF')} style={{ margin: '10px', padding: '10px 20px' }}>
        Turn OFF
      </button>
    </div>
  );
};

export default ValveControl;
