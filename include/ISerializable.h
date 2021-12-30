#ifndef ISERIALIZABLE_H
#define ISERIALIZABLE_H

namespace file {

    // Интерфейс Сериализации - преобразование объекта в поток байтов
    class ISerializable {
    };

    //class ISerializable
    //{
    //protected:
    //    ~ISerializable() = default;
    //
    //public:
    //    virtual auto Serialize(std::ostream& os) const->std::size_t = 0;
    //    virtual auto UnSerialize(std::istream& is)->std::size_t = 0;
    //    virtual auto serialized_size() const noexcept -> std::size_t = 0;
    //};

}

#endif // !_H