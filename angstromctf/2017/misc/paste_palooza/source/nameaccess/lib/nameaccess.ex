defmodule Nameaccess do

def main(args) do
    unsafeName = Enum.at(args,0) <> ".txt"
    fileName = filter(unsafeName <> <<0>>, "", String.length(unsafeName))
    case File.read fileName do
        {:ok, content} -> IO.puts content
        {:error, reason} -> IO.puts "file not found"
    end
end

def filter(<< head, tail :: binary >>, acc, n) do
    if n == 0 do
        acc
    else
        n = n - 1
        if head < 32 or head > 126 or head == 47 do
            filter(tail, acc, n)
        else
            filter(tail, acc <> <<head>>, n)
        end
    end
end

end
