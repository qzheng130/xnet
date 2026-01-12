package com.chenshuo.xnet.example.multiplexer.testcase;

import java.util.regex.Matcher;

import com.chenshuo.xnet.example.multiplexer.DataEvent;
import com.chenshuo.xnet.example.multiplexer.Event;
import com.chenshuo.xnet.example.multiplexer.EventSource;
import com.chenshuo.xnet.example.multiplexer.MockClient;
import com.chenshuo.xnet.example.multiplexer.TestCase;

public class TestOneClientNoData extends TestCase {

    @Override
    public void run() {
        if (!queue.isEmpty())
            fail("EventQueue is not empty");
        
        MockClient client = god.newClient();
        Event ev = queue.take();
        DataEvent de = (DataEvent) ev;
        assertEquals(EventSource.kBackend, de.source);

        Matcher m = god.commandChannel.matcher(de.getString());
        if (!m.matches())
            fail("command channel message doesn't match.");
        
        int connId = Integer.parseInt(m.group(1));
        assertTrue(connId > 0);
        client.setId(connId);
        
        assertEquals("UP", m.group(2));
        
        client.disconnect();
        de = (DataEvent) queue.take();
        assertEquals(EventSource.kBackend, de.source);
        m = god.commandChannel.matcher(de.getString());
        if (!m.matches())
            fail("command channel message doesn't match.");
        
        assertEquals(connId, Integer.parseInt(m.group(1)));
        assertEquals("DOWN", m.group(2));
    }
}
