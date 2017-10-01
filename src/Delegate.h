#ifndef DELEGATE_H
#define DELEGATE_H

template<typename R = void, typename ... Ps> class DelegateBase
{
  // Operatoren
public:
  virtual R operator()(Ps ...) = 0;
};

template<typename T, typename R = void, typename ... Ps> class DelegateMember : public DelegateBase<R, Ps ...>
{
  // Felder
private:
  T* Obj;
  R (T::*Fp)(Ps ...);

  // Konstruktor
public:
  DelegateMember(T* obj, R (T::*fp)(Ps ...))
    : Obj(obj), Fp(fp)
  {

  }

  // Methoden
public:
  virtual R operator()(Ps ... params)
  {
      return (Obj->*Fp)(params ...);
  }
};
template<typename T, typename R = void, typename ... Ps> class DelegateLambda : public DelegateBase<R, Ps ...>
{
  // Felder
private:
  T Expr;

  // Konstruktor
public:
  DelegateLambda(T expr)
    : Expr(expr)
  {

  }

  // Methoden
public:
  virtual R operator()(Ps ... params)
  {
      return Expr(params ...);
  }
};
template<typename R = void, typename ... Ps> class DelegateStatic : public DelegateBase<R, Ps ...>
{
  // Felder
private:
  R (*Fp)(Ps ...);

  // Konstruktor
public:
  DelegateStatic(R (*fp)(Ps ...))
    : Fp(fp)
  {

  }

  // Methoden
public:
  virtual R operator()(Ps ... params)
  {
      return Fp(params ...);
  }
};

template<typename R = void, typename ... Ps> class Delegate
{
  // Felder
private:
  DelegateBase<R, Ps ...>* internal;

  // Konstruktor
public:
  template<typename T> Delegate(T* obj, R (T::*fp)(Ps ...))
    : internal(new DelegateMember<T, R, Ps ...>(obj, fp))
  {

  }
  Delegate(R (*fp)(Ps ...))
    : internal(new DelegateStatic<R, Ps ...>(fp))
  {

  }
  template<typename T> Delegate(T expr)
    : internal(new DelegateLambda<T, R, Ps ...>(expr))
  {

  }

  // Operatoren
public:
  R operator()(Ps ... params)
  {
    return (*internal)(params ...);
  }
};

#endif
