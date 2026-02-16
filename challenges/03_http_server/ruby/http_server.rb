#!/usr/bin/env ruby
# frozen_string_literal: true

# HTTP Server - Ruby 実装 (標準ライブラリのみ)

require 'socket'
require 'json'

puts "=== HTTP Server Demo ===\n\n"

# --- Request Parser ---
class Request
  attr_reader :method, :path, :headers, :body

  def initialize(method:, path:, headers: {}, body: nil)
    @method = method
    @path = path
    @headers = headers
    @body = body
  end

  def self.parse(client)
    request_line = client.gets
    return nil if request_line.nil?

    method, path, _version = request_line.split

    headers = {}
    while (line = client.gets) && line != "\r\n"
      key, value = line.split(': ', 2)
      headers[key.downcase] = value&.strip
    end

    body = nil
    if headers['content-length']
      body = client.read(headers['content-length'].to_i)
    end

    new(method: method, path: path, headers: headers, body: body)
  end

  def params
    return {} unless path.include?('?')

    query = path.split('?', 2).last
    query.split('&').to_h do |pair|
      key, value = pair.split('=', 2)
      [key, value || '']
    end
  end
end

# --- Response Builder ---
class Response
  attr_accessor :status, :headers, :body

  STATUS_MESSAGES = {
    200 => 'OK',
    201 => 'Created',
    400 => 'Bad Request',
    404 => 'Not Found',
    405 => 'Method Not Allowed',
    500 => 'Internal Server Error'
  }.freeze

  def initialize(status: 200)
    @status = status
    @headers = { 'Content-Type' => 'text/plain; charset=utf-8' }
    @body = ''
  end

  def text(content)
    @headers['Content-Type'] = 'text/plain; charset=utf-8'
    @body = content
    self
  end

  def json(data)
    @headers['Content-Type'] = 'application/json; charset=utf-8'
    @body = data.to_json
    self
  end

  def html(content)
    @headers['Content-Type'] = 'text/html; charset=utf-8'
    @body = content
    self
  end

  def to_s
    @headers['Content-Length'] = @body.bytesize.to_s
    @headers['Connection'] = 'close'

    status_message = STATUS_MESSAGES[@status] || 'Unknown'
    response = "HTTP/1.1 #{@status} #{status_message}\r\n"
    @headers.each { |k, v| response += "#{k}: #{v}\r\n" }
    response += "\r\n"
    response += @body
    response
  end
end

# --- Router ---
class Router
  def initialize
    @routes = Hash.new { |h, k| h[k] = {} }
  end

  def get(path, &handler)
    @routes['GET'][path] = handler
  end

  def post(path, &handler)
    @routes['POST'][path] = handler
  end

  def route(request)
    # 完全一致
    handler = @routes[request.method][request.path.split('?').first]
    return handler.call(request) if handler

    # パターンマッチ (例: /hello/:name)
    @routes[request.method].each do |pattern, handler|
      if pattern.include?(':')
        regex = Regexp.new("^#{pattern.gsub(/:(\w+)/, '(?<\1>[^/]+)')}$")
        match = regex.match(request.path.split('?').first)
        if match
          request.define_singleton_method(:path_params) { match.named_captures }
          return handler.call(request)
        end
      end
    end

    # 404
    Response.new(status: 404).text("Not Found: #{request.path}")
  end
end

# --- Server ---
class HTTPServer
  def initialize(port: 8080)
    @port = port
    @router = Router.new
  end

  def get(path, &handler)
    @router.get(path, &handler)
  end

  def post(path, &handler)
    @router.post(path, &handler)
  end

  def start
    server = TCPServer.new('127.0.0.1', @port)
    puts "Server started at http://127.0.0.1:#{@port}"
    puts "Try:"
    puts "  curl http://localhost:#{@port}/"
    puts "  curl http://localhost:#{@port}/hello/world"
    puts "  curl http://localhost:#{@port}/json"
    puts "\nPress Ctrl+C to stop\n\n"

    loop do
      client = server.accept
      handle_client(client)
    rescue Interrupt
      puts "\nShutting down..."
      break
    end
  ensure
    server&.close
  end

  private

  def handle_client(client)
    request = Request.parse(client)
    return if request.nil?

    puts "#{request.method} #{request.path}"

    response = @router.route(request)
    client.print(response.to_s)
  rescue StandardError => e
    puts "Error: #{e.message}"
    error_response = Response.new(status: 500).text("Internal Server Error")
    client.print(error_response.to_s)
  ensure
    client.close
  end
end

# --- メイン ---
if __FILE__ == $PROGRAM_NAME
  server = HTTPServer.new(port: 8080)

  # ルート定義
  server.get '/' do |_req|
    Response.new.text("Welcome to Ruby HTTP Server!")
  end

  server.get '/hello/:name' do |req|
    name = req.path_params['name']
    Response.new.text("Hello, #{name}!")
  end

  server.get '/json' do |_req|
    Response.new.json({ message: 'Hello, JSON!', status: 'ok' })
  end

  server.get '/html' do |_req|
    Response.new.html(<<~HTML)
      <!DOCTYPE html>
      <html>
        <head><title>Ruby Server</title></head>
        <body><h1>Hello from Ruby!</h1></body>
      </html>
    HTML
  end

  server.get '/echo' do |req|
    Response.new.json(req.params)
  end

  server.start
end
