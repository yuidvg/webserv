require 'open3'

class ChunkTest
  def initialize(host, port)
    @host = host
    @port = port
    @cmd = "nc"
  end

  def run
    Open3.popen3("#{@cmd} #{@host} #{@port}") do |stdin, stdout, stderr, wait_thr|
      begin
        i = 0
        loop do
          body = "hogehoge#{i}\nfugafuga#{i}"
          size = body.size
          head = \
            "POST /upload HTTP/1.1\r\nUser-Agent: curl/7.28.0\r\nHost: #{@host}:#{@port}\r\nContent-type: text/plain\r\nTransfer-Encoding: chunked\r\nConnection: Keep-Alive\r\nExpect: 100-continue\r\n\r\n"
          if i == 0
            stdin << head
          elsif i > 3
            stdin << "0\r\n\r\n"
            break
          else
            chunk = "#{size.to_s(16)}\r\n#{body}\r\n"
            stdin << chunk
            sleep 1
          end
          i += 1
        end

      ensure
        # 明示的にプロセスを終了させる
        Process.kill("TERM", wait_thr.pid)
        wait_thr.value # プロセスの終了を待つ
      end
    end
  end
end

chunk = ChunkTest.new('localhost', 8080)
chunk.run
