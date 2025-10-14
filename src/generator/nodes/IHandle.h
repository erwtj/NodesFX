#ifndef IHANDLE_H
#define IHANDLE_H

class IHandle {
public:
    enum class HandleType {
        Input,
        Output
    };

    enum class DataType {
        Float,
        Vec2,
        Vec3,
        Vec4,
        Color,
        Texture
    };

    IHandle(const HandleType type, const DataType dataType) : _type(type), _dataType(dataType) {}
    virtual ~IHandle() = default;

    [[nodiscard]] HandleType GetType() const { return _type; }
    [[nodiscard]] DataType GetDataType() const { return _dataType; }

    virtual void Reset() = 0;

private:
    const HandleType _type;
    const DataType _dataType;
};

#endif //IHANDLE_H
