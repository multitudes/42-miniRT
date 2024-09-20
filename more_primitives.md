#  Solution for tri, ellipse, annulus primitives

## more primitives from the book 

This code was kind of hidden in the git repo. They did not use it so i had to search for a tag like:

To check a tag on GitHub, you can use the Git command line interface (CLI) or the GitHub web interface. Here are the steps for both methods:

### Using Git Command Line Interface

1. **List All Tags:**
   To list all tags in the repository, use:
   ```sh
   git tag
   ```

2. **Show Tag Details:**
   To show details of a specific tag, use:
   ```sh
   git show <tag-name>
   ```

#### Using Git CLI
```sh
# List all tags
git tag

# Show details of a specific tag
git show v1.0.0
```

## the code

this is the code that I found and will keep here for inspiration and further testing eventually

```cpp
class tri : public quad {
  public:
    tri(const point3& o, const vec3& aa, const vec3& ab, shared_ptr<material> m)
      : quad(o, aa, ab, m)
    {}

    virtual bool hit_ab(double a, double b, hit_record& rec) const override {
        if ((a < 0) || (b < 0) || (a + b > 1))
            return false;

        rec.u = a;
        rec.v = b;
        return true;
    }
};


class ellipse : public quad {
  public:
    ellipse(
        const point3& center, const vec3& side_A, const vec3& side_B, shared_ptr<material> m
    ) : quad(center, side_A, side_B, m)
    {}
    virtual void set_bounding_box() override {
        bbox = aabb(plane_origin - axis_A - axis_B, plane_origin + axis_A + axis_B).pad();
    }

    virtual bool hit_ab(double a, double b, hit_record& rec) const override {
        if ((a*a + b*b) > 1)
            return false;

        rec.u = a/2 + 0.5;
        rec.v = b/2 + 0.5;
        return true;
    }
};


class annulus : public quad {
  public:
    annulus(
        const point3& center, const vec3& side_A, const vec3& side_B, double _inner,
        shared_ptr<material> m)
      : quad(center, side_A, side_B, m), inner(_inner)
    {}

    virtual void set_bounding_box() override {
        bbox = aabb(plane_origin - axis_A - axis_B, plane_origin + axis_A + axis_B).pad();
    }

    virtual bool hit_ab(double a, double b, hit_record& rec) const override {
        auto center_dist = sqrt(a*a + b*b);
        if ((center_dist < inner) || (center_dist > 1))
            return false;

        rec.u = a/2 + 0.5;
        rec.v = b/2 + 0.5;
        return true;
    }

  private:
    double inner;
};
```